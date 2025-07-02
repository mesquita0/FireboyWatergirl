/**********************************************************************************
// Scene (Código Fonte)
// 
// Criação:     16 Mar 2012
// Atualização: 28 Set 2023
// Compilador:  Visual C++ 2022
//
// Descrição:   Define uma classe para gerenciar o cenário do jogo.
//
//              Um gerenciador de cena é responsável por guardar os objetos
//              da cena, atualizando-os e desenhando-os de forma mais prática.
//              Ele pode ser usado também para outras tarefas que impliquem em
//              percorrer a lista de objetos, como detecção de colisão.
//
**********************************************************************************/

#include "Engine.h"
#include "Scene.h"
#include "Object.h"
#include "FireboyWatergirl.h"

// ---------------------------------------------------------------------------------

float dotProduct(const Point& a, const Point& b);
Point getNormal(Point& v1, Point& v2);
float project(const Point vertex, const Point& axis);
float getOverlap(Point axis, Poly* c, Poly* polygonB);

// ---------------------------------------------------------------------------------

Scene::Scene()
{

}

// ---------------------------------------------------------------------------------

Scene::~Scene()
{
    // remove da memória os objetos estáticos
    for (auto obj : statics)
        delete obj;

    // remove da memória os objetos em movimento
    for (auto obj : moving)
        delete obj;
}

// ---------------------------------------------------------------------------------


void Scene::Add(Object* obj, int type)
{
    // insere novo objeto na cena
    if (type == STATIC)
        statics.push_back(obj);
    else
        moving.push_back(obj);
}

// ---------------------------------------------------------------------------------

void Scene::Remove(Object* obj, int type)
{
    // remove objeto da cena
    if (type == STATIC)
        statics.remove(obj);
    else
        moving.remove(obj);
}

// ---------------------------------------------------------------------------------

void Scene::Remove()
{
    // remove objeto da cena
    if (processing == STATIC)
        statics.remove(*it);
    else
        moving.remove(*it);
}

// ---------------------------------------------------------------------------------

void Scene::Delete(Object* obj, int type)
{
    toDelete.push_back(ObjectDel{ obj, type });
}

// ---------------------------------------------------------------------------------

void Scene::Delete()
{
    toDelete.push_back(ObjectDel{ *it, processing });
}

// ---------------------------------------------------------------------------------

uint Scene::Size()
{
    return uint(moving.size() + statics.size());
}

// ---------------------------------------------------------------------------------

void Scene::ProcessDeleted()
{
    // remove objetos duplicados
    toDelete.sort();
    toDelete.unique();

    for (const auto & [obj, type] : toDelete)
    {
        // libera memória ocupada pelo objeto
        delete obj;

        // remove objeto da cena
        if (type == STATIC)
            statics.remove(obj);
        else
            moving.remove(obj);
    }

    // esvazia a lista 
    toDelete.clear();
}

// ---------------------------------------------------------------------------------

void Scene::Update()
{
    // atualiza todos os objetos estáticos
    processing = STATIC;
    for (it = statics.begin(); it != statics.end(); ++it)
        (*it)->Update();

    // atualiza todos os objetos em movimento
    processing = MOVING;
    for (it = moving.begin(); it != moving.end(); ++it)
        (*it)->Update();

    ProcessDeleted();
}

// ---------------------------------------------------------------------------------

void Scene::Draw()
{
    // desenha todos os objetos estáticos
    processing = STATIC;
    for (it = statics.begin(); it != statics.end(); ++it)
        (*it)->Draw();

    // desenha todos os objetos em movimento
    processing = MOVING;
    for (it = moving.begin(); it != moving.end(); ++it)
        (*it)->Draw();
}

// ---------------------------------------------------------------------------------

void Scene::DrawBBox()
{
    // inicia desenho de pixels
    Engine::renderer->BeginPixels();

    // desenha bounding box dos objetos estáticos
    for (const auto& obj : statics)
    {
        if (obj->BBox())
        {
            Engine::renderer->Draw(obj->BBox(), 0xffff00ff);
        }
    }

    // desenha bounding box dos objetos em movimento
    for (const auto& obj : moving)
    {
        if (obj->BBox())
        {
            Engine::renderer->Draw(obj->BBox(), 0xffff00ff);
        }
    }

    // finaliza desenho de pixels
    Engine::renderer->EndPixels();
}

// ---------------------------------------------------------------------------------

void Scene::Begin()
{
    // aponta para o primeiro elemento de cada lista
    its = statics.begin();
    itm = moving.begin();
    processing = STATIC;
}

// ---------------------------------------------------------------------------------

Object * Scene::Next()
{
    // se apontador aponta para objeto válido
    if (its != statics.end())
    {
        // passa ao próximo objeto
        // guarda apontador para o anterior
        it = its++;
        return *it;
    }
    else
        if (itm != moving.end())
        {
            processing = MOVING;
            it = itm++;
            return *it;
        }
        else
            // chegou ao fim das listas
            return nullptr;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Point* p, Point* q)
{
    // se as coordenadas inteiras são iguais 
    if (int(p->X()) == int(q->X()) && int(p->Y()) == int(q->Y()))
        return true;

    // caso contrário não há colisão
    return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Point * p, Rect * r)
{
    // se as coordenadas  do ponto estão dentro do retângulo
    if (p->X() >= r->Left() && p->X() <= r->Right())
        if (p->Y() >= r->Top() && p->Y() <= r->Bottom())
            return true;

    // caso contrário não há colisão
    return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Point * p, Circle * c)
{
    // se a distância entre o ponto e o centro do círculo
    // for menor que o raio do círculo então há colisão
    if (Point::Distance(*p, Point(c->CenterX(), c->CenterY())) <= c->Radius())
        return true;
    else
        return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Point* p, Poly* pol)
{
    // Unimplemented
    return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Rect * ra, Rect * rb)
{
    // verificando sobreposição no eixo x
    bool overlapX = (rb->Left() <= ra->Right() && ra->Left() <= rb->Right());

    // verificando sobreposição no eixo y
    bool overlapY = (rb->Top() <= ra->Bottom() && ra->Top() <= rb->Bottom());

    // existe colisão se há sobreposição nos dois eixos
    return overlapX && overlapY;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Rect * r, Circle * c)
{
    // encontra o ponto do retângulo mais próximo do centro do círculo
    float px, py;

    // eixo x
    if (c->CenterX() < r->Left())
        px = r->Left();
    else
        if (c->CenterX() > r->Right())
            px = r->Right();
        else
            px = c->CenterX();

    // eixo y
    if (c->CenterY() < r->Top())
        py = r->Top();
    else
        if (c->CenterY() > r->Bottom())
            py = r->Bottom();
        else
            py = c->CenterY();

    // verifica se há colisão entre este ponto e o círculo
    Point point(px, py);
    return Collision(&point, c);
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Rect * r, Poly * pol)
{
    Point points[4] = {
        { r->Left(),  r->Top() },
        { r->Left(),  r->Bottom() },
        { r->Right(), r->Bottom() },
        { r->Right(), r->Top() }
    };
    Poly rect = Poly(points, 4);

    return Collision(pol, &rect, r);
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Circle * ca, Circle * cb)
{
    // deltas podem ser negativos se a subtração é feita na ordem errada
    // levando essa possibilidade em conta é melhor pegar os valores absolutos
    float deltaX = abs(ca->CenterX() - cb->CenterX());
    float deltaY = abs(ca->CenterY() - cb->CenterY());

    // calcule a distância entre os centros dos círculos
    float distance = float(sqrt(double(deltaX) * double(deltaX) + double(deltaY) * double(deltaY)));

    // se a distância é menor que a soma dos raios
    // existe colisão entre os círculos
    if (distance <= (ca->Radius() + cb->Radius()))
        return true;

    // nenhum colisão detectada
    return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Circle* c, Poly* pol)
{
    // Unimplemented
    return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Poly* pa, Poly* pb, Rect * r = nullptr)
{
    float min_overlap = FLT_MAX;
    Point axis_min_overlap = { };

    // Test axes from pa
    for (int i = 0; i < pa->vertexCount; ++i) {
        Point v1 = pa->vertexList[i];
        Point v2 = pa->vertexList[(i + 1) % pa->vertexCount];
        v1.Translate(pa->X(), pa->Y());
        v2.Translate(pa->X(), pa->Y());

        Point axis = getNormal(v1, v2);
        float overlap = getOverlap(axis, pa, pb);
        if (overlap < 0) return false; // Separating axis found, no collision

        if (overlap < min_overlap) {
            min_overlap = overlap;
            axis_min_overlap = axis;
        }
    }

    // Test axes from pb 
    for (int i = 0; i < pb->vertexCount; ++i) {
        Point v1 = pb->vertexList[i];
        Point v2 = pb->vertexList[(i + 1) % pb->vertexCount];
        v1.Translate(pb->X(), pb->Y());
        v2.Translate(pb->X(), pb->Y());

        Point axis = getNormal(v1, v2);
        float overlap = getOverlap(axis, pa, pb);
        if (overlap < 0) return false; // Separating axis found, no collision

        if (overlap < min_overlap) {
            min_overlap = overlap;
            axis_min_overlap = axis;
        }
    }

    bool is_fireboy = (r && r == FireboyWatergirl::fireboy->BBox());
    Vector* mtv_pa = is_fireboy ? &pa->mtv_fire : &pa->mtv_water;
    Vector* mtv_pb = is_fireboy ? &pb->mtv_fire : &pb->mtv_water;
    
    // Save axis with minimum overlap to geometries
    mtv_pa->XComponent(axis_min_overlap.X());
    mtv_pa->YComponent(axis_min_overlap.Y());
    mtv_pb->XComponent(axis_min_overlap.X());
    mtv_pb->YComponent(axis_min_overlap.Y());
    
    // Compute minimum translation vector
    mtv_pa->ScaleTo(min_overlap);
    mtv_pb->ScaleTo(min_overlap);

    // Direction of mtv
    Point centers_vector = { pb->center.X() - pa->center.X(), pb->center.Y() - pa->center.Y() };
    if (dotProduct(centers_vector, axis_min_overlap) < 0) {
        mtv_pa->Scale(-1);
        mtv_pb->Scale(-1);
    }

    return true; // No separating axis found, collision detected
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Mixed * m, Geometry * s)
{
    bool collision = false;

    // percorra lista até achar uma colisão
    auto i = m->shapes.begin();

    while (!collision && i != m->shapes.end())
    {
        switch ((*i)->Type())
        {
            // Point
        case POINT_T:
            switch (s->Type())
            {
            case POINT_T:     collision = Collision((Point*)(*i), (Point*)s); break;
            case CIRCLE_T:    collision = Collision((Point*)(*i), (Circle*)s); break;
            case RECTANGLE_T: collision = Collision((Point*)(*i), (Rect*)s); break;
            case POLYGON_T:   collision = Collision((Point*)(*i), (Poly*)s); break;
            case MIXED_T:     collision = Collision((Point*)(*i), (Mixed*)s); break;
            }
            break;

            // Circle
        case CIRCLE_T:
            switch (s->Type())
            {
            case POINT_T:     collision = Collision((Circle*)(*i), (Point*)s); break;
            case CIRCLE_T:    collision = Collision((Circle*)(*i), (Circle*)s); break;
            case RECTANGLE_T: collision = Collision((Circle*)(*i), (Rect*)s); break;
            case POLYGON_T:   collision = Collision((Circle*)(*i), (Poly*)s); break;
            case MIXED_T:     collision = Collision((Circle*)(*i), (Mixed*)s); break;
            }
            break;

            // Rectangle
        case RECTANGLE_T:
            switch (s->Type())
            {
            case POINT_T:     collision = Collision((Rect*)(*i), (Point*)s); break;
            case CIRCLE_T:    collision = Collision((Rect*)(*i), (Circle*)s); break;
            case RECTANGLE_T: collision = Collision((Rect*)(*i), (Rect*)s); break;
            case POLYGON_T:   collision = Collision((Rect*)(*i), (Poly*)s); break;
            case MIXED_T:     collision = Collision((Rect*)(*i), (Mixed*)s); break;
            }
            break;

            // Polygon
        case POLYGON_T:
            switch (s->Type())
            {
            case POINT_T:     collision = Collision((Poly*)(*i), (Point*)s); break;
            case CIRCLE_T:    collision = Collision((Poly*)(*i), (Circle*)s); break;
            case RECTANGLE_T: collision = Collision((Poly*)(*i), (Rect*)s); break;
            case POLYGON_T:   collision = Collision((Poly*)(*i), (Poly*)s); break;
            case MIXED_T:     collision = Collision((Poly*)(*i), (Mixed*)s); break;
            }
            break;

            // Mixed
        case MIXED_T:
            collision = Collision((Mixed*)(*i), s);
            break;
        }

        // passa para a próxima geometria da forma mista
        ++i;
    }

    return collision;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Object* oa, Object* ob)
{
    // um dos objetos não tem bounding box
    if (!(oa->BBox() && ob->BBox()))
        return false;

    switch (oa->BBox()->Type())
    {
        // Point    
    case POINT_T:

        switch (ob->BBox()->Type())
        {
        case POINT_T:
            // Point && Point
            return Collision((Point*)oa->BBox(), (Point*)ob->BBox());

        case CIRCLE_T:
            // Point && Circle
            return Collision((Point*)oa->BBox(), (Circle*)ob->BBox());

        case RECTANGLE_T:
            // Point && Rectangle
            return Collision((Point*)oa->BBox(), (Rect*)ob->BBox());

        case POLYGON_T:
            // Point && Polygon
            return Collision((Point*)oa->BBox(), (Poly*)ob->BBox());

        case MIXED_T:
            // Point && Mixed
            return Collision(oa->BBox(), (Mixed*)ob->BBox());

        default:
            // Point && Unknown
            return false;
        }

        // Circle    
    case CIRCLE_T:

        switch (ob->BBox()->Type())
        {
        case POINT_T:
            // Circle && Point
            return Collision((Circle*)oa->BBox(), (Point*)ob->BBox());

        case CIRCLE_T:
            // Circle && Circle
            return Collision((Circle*)oa->BBox(), (Circle*)ob->BBox());

        case RECTANGLE_T:
            // Circle && Rectangle
            return Collision((Circle*)oa->BBox(), (Rect*)ob->BBox());

        case POLYGON_T:
            // Circle && Polygon
            return Collision((Circle*)oa->BBox(), (Poly*)ob->BBox());

        case MIXED_T:
            // Circle && Mixed
            return Collision(oa->BBox(), (Mixed*)ob->BBox());

        default:
            // Circle && Unknown
            return false;
        }

        // Rectangle
    case RECTANGLE_T:

        switch (ob->BBox()->Type())
        {
        case POINT_T:
            // Rectangle && Point
            return Collision((Rect*)oa->BBox(), (Point*)ob->BBox());

        case CIRCLE_T:
            // Rectangle && Circle
            return Collision((Rect*)oa->BBox(), (Circle*)ob->BBox());

        case RECTANGLE_T:
            // Rectangle && Rectangle
            return Collision((Rect*)oa->BBox(), (Rect*)ob->BBox());

        case POLYGON_T:
            // Rectangle && Polygon
            return Collision((Rect*)oa->BBox(), (Poly*)ob->BBox());

        case MIXED_T:
            // Rectangle && Mixed
            return Collision(oa->BBox(), (Mixed*)ob->BBox());

        default:
            // Rectangle && Unknown
            return false;
        }

        // Polygon
    case POLYGON_T:

        switch (ob->BBox()->Type())
        {
        case POINT_T:
            // Polygon && Point
            return Collision((Poly*)oa->BBox(), (Point*)ob->BBox());

        case CIRCLE_T:
            // Polygon && Circle
            return Collision((Poly*)oa->BBox(), (Circle*)ob->BBox());

        case RECTANGLE_T:
            // Polygon && Rectangle
            return Collision((Poly*)oa->BBox(), (Rect*)ob->BBox());

        case POLYGON_T:
            // Polygon && Polygon
            return Collision((Poly*)oa->BBox(), (Poly*)ob->BBox());

        case MIXED_T:
            // Polygon && Mixed
            return Collision(oa->BBox(), (Mixed*)ob->BBox());

        default:
            // Polygon && Unknown
            return false;
        }

        // Mixed
    case MIXED_T:
        // Mixed && Geometry
        return Collision((Mixed*)oa->BBox(), ob->BBox());


        // Unknown
    default:
        return false;
    }
}

// ---------------------------------------------------------------------------------

void Scene::CollisionDetection()
{
    // iteradores para as listas
    list<Object*>::iterator start, end;

    // limpa lista de colisões
    collisions.clear();

    // -------------------
    // Detecção da colisão
    // -------------------

    // se existe pelo menos dois objetos que se movem, teste a colisão entre eles
    if (moving.size() >= 2)
    {
        // pega iterador para último elemento da lista
        end = moving.end();
        end--;

        // testa colisão entre todos os pares de objetos que se movem
        for (auto i = moving.begin(); i != end; ++i)
        {
            start = i;

            // j inicia no segundo elemento da lista
            for (auto j = ++start; j != moving.end(); ++j)
            {
                if (Collision(*i, *j))
                    collisions.push_back(ObjectPair(*i, *j));
            }
        }
    }

    // testa colisão entre objetos que se movem e objetos estáticos
    for (auto i : moving)
    {
        for (auto j : statics)
        {
            if (Collision(i, j))
                collisions.push_back(ObjectPair(i, j));
        }
    }

    // --------------------
    // Resolução da colisão
    // --------------------

    if (!collisions.empty())
    {
        for (const auto & [a,b] : collisions)
        {
            a->OnCollision(b);
            b->OnCollision(a);
        }
    }

    ProcessDeleted();
}

// --------------------------------------------------------------------------------

float dotProduct(const Point& a, const Point& b) {
    return a.X() * b.X() + a.Y() * b.Y();
}

Point getNormal(Point& v1, Point& v2) {
    return { -(v2.Y() - v1.Y()), v2.X() - v1.X() };
}

float project(const Point vertex, const Point& axis) {
    float len = sqrt(dotProduct(axis, axis));
    return dotProduct(vertex, axis) / len;
}

float getOverlap(Point axis, Poly* polygonA, Poly* polygonB) {
    float minA = FLT_MAX;
    float maxA = -FLT_MAX;
    float minB = FLT_MAX;
    float maxB = -FLT_MAX;

    for (int i = 0; i < polygonA->vertexCount; i++) {
        Point p = polygonA->vertexList[i];
        p.Translate(polygonA->X(), polygonA->Y());
        float projection = project(p, axis);
        minA = min(minA, projection);
        maxA = max(maxA, projection);
    }

    for (int i = 0; i < polygonB->vertexCount; i++) {
        Point p = polygonB->vertexList[i];
        p.Translate(polygonB->X(), polygonB->Y());
        float projection = project(p, axis);
        minB = min(minB, projection);
        maxB = max(maxB, projection);
    }

    return min(maxA, maxB) - max(minA, minB);
}
