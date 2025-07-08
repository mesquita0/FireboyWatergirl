#include "WorldEntity.h"
#include "FireboyWatergirl.h"

WorldEntity::WorldEntity(float posX, float posY, float posZ, EntityTypeSprite platType, float scale, float rotation, Color tint)
    : scale(scale), rotation(rotation), color(tint)
{
    switch (platType)
    {
    case GROUND1:          type = GROUND;              entity = new Sprite("Resources/Platforms/Madeira/PisoMadeira.png"); break;
    case PLATWOOD1:        type = GROUND;              entity = new Sprite("Resources/Platforms/Madeira/MeiaPlataformaMadeira.png"); break;
    case PLATWOOD2:        type = GROUND;              entity = new Sprite("Resources/Platforms/Madeira/MeiaPlataformaMadeira2.png"); break;
    case PLATWOOD3:        type = GROUND;              entity = new Sprite("Resources/Platforms/Madeira/MeiaPlataformaMadeira4.png"); break;
    case PLATWOOD4:        type = GROUND;              entity = new Sprite("Resources/Platforms/Madeira/PlataformaMadeiraH.png"); break;
    case PLATWOOD5:        type = GROUND;              entity = new Sprite("Resources/Platforms/Madeira/PilarMadeira3.png"); break;
    case PLATWOOD6:        type = GROUND;              entity = new Sprite("Resources/Platforms/Madeira/ParedeMadeira.png"); break;
    case MOVABLE_BOX_WOOD: type = MOVABLE_BOX;         entity = new Sprite("Resources/Platforms/Madeira/CaixaMadeira.png"); break;
    case FINISH_PORTAL1:   type = FINISH_PORTAL_FIRE;  entity = new Sprite("Resources/FireDoor.png"); break;
    case FINISH_PORTAL2:   type = FINISH_PORTAL_WATER; entity = new Sprite("Resources/WaterDoor.png"); break;
    case THORN1:           type = THORN;               entity = new Sprite("Resources/Espinhos/Espinho.png"); break;
    case THORN2:           type = THORN;               entity = new Sprite("Resources/Espinhos/Espinho6.png"); break;
    case THORN3:           type = THORN;               entity = new Sprite("Resources/Espinhos/Espinho7.png"); break;
    }

    if (entity) {
        width  = entity->Width();
        height = entity->Height();
    }
    
    Point points[4];
    switch (type) {
    case GROUND:
    case FINISH_PORTAL_FIRE:
    case FINISH_PORTAL_WATER:
    case MOVABLE_BOX:
        points[0] = { -entity->Width() / 2.0f, -entity->Height() / 2.0f };
        points[1] = {  entity->Width() / 2.0f, -entity->Height() / 2.0f };
        points[3] = { -entity->Width() / 2.0f,  entity->Height() / 2.0f };
        points[2] = {  entity->Width() / 2.0f,  entity->Height() / 2.0f };

        BBox(new Poly(points, 4));

        BBox()->RotateTo(rotation);
        BBox()->ScaleTo(scale);

        break;

    case THORN:
        points[0] = { -entity->Width() / 2.0f, entity->Height() / 2.0f };
        points[1] = { entity->Width() / 2.0f, entity->Height() / 2.0f };
        points[2] = { 0.0, -entity->Height() / 2.0f };
        BBox(new Poly(points, 3));
        break;
    }

    MoveTo(posX, posY, posZ);
}

WorldEntity::~WorldEntity()
{
    delete entity;
}

void WorldEntity::Update()
{
    
}

void WorldEntity::OnCollision(Object* obj) {
    switch (type)
    {
    case MOVABLE_BOX:
        if (obj->Type() == GROUND) {
            // Mantém caixa fora do chão (pode colidir apenas no eixo x)
            Translate(-obj->BBox()->mtv_water.XComponent(), 0);

            // Quando a caixa colide uma vez, não é mais possível move-la
            if (abs(obj->BBox()->mtv_water.XComponent()) > 1) type = GROUND;
        }
        break;
    
    default:
        break;
    }
}
