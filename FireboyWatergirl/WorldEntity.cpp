#include "WorldEntity.h"
#include "FireboyWatergirl.h"

WorldEntity::WorldEntity(float posX, float posY, float posZ, EntityTypeSprite platType, float scale, float rotation, Color tint)
    : scale(scale), rotation(rotation), color(tint)
{
    switch (platType)
    {
    case GROUND1:            type = GROUND;              entity = new Sprite("Resources/Platforms/Madeira/PisoMadeira.png"); break;
    case PLATWOOD1:          type = GROUND;              entity = new Sprite("Resources/Platforms/Madeira/MeiaPlataformaMadeira.png"); break;
    case PLATWOOD2:          type = GROUND;              entity = new Sprite("Resources/Platforms/Madeira/MeiaPlataformaMadeira2.png"); break;
    case PLATWOOD3:          type = GROUND;              entity = new Sprite("Resources/Platforms/Madeira/MeiaPlataformaMadeira4.png"); break;
    case PLATWOOD4:          type = GROUND;              entity = new Sprite("Resources/Platforms/Madeira/PlataformaMadeiraH.png"); break;
    case PLATWOOD5:          type = GROUND;              entity = new Sprite("Resources/Platforms/Madeira/PilarMadeira3.png"); break;
    case PLATWOOD6:          type = GROUND;              entity = new Sprite("Resources/Platforms/Madeira/ParedeMadeira.png"); break;
    case MOVABLE_BOX_WOOD:   type = MOVABLE_BOX;         entity = new Sprite("Resources/Platforms/Madeira/CaixaMadeira.png"); break;
    case FINISH_PORTAL1:     type = FINISH_PORTAL_FIRE;  entity = new Sprite("Resources/FireDoor.png"); break;
    case FINISH_PORTAL2:     type = FINISH_PORTAL_WATER; entity = new Sprite("Resources/WaterDoor.png"); break;
    case THORN1:             type = THORN;               entity = new Sprite("Resources/Espinhos/Espinho.png"); break;
    case THORN2:             type = THORN;               entity = new Sprite("Resources/Espinhos/Espinho6.png"); break;
    case THORN3:             type = THORN;               entity = new Sprite("Resources/Espinhos/Espinho7.png"); break;
    case MOVING_PLATFORM_X1: type = MOVING_PLATFORM_X;   entity = new Sprite("Resources/Platforms/Madeira/MeiaPlataformaMadeira4.png"); break;
    case MOVING_PLATFORM_Y1: type = MOVING_PLATFORM_Y;   entity = new Sprite("Resources/Platforms/Madeira/MeiaPlataformaMadeira4.png"); break;
    case FINISH_PORTAL_3:    type = FINISH_PORTAL_ANY;   entity = new Sprite("Resources/Door.png"); break;
    case ROTATING_PLATFORM1: type = ROTATING_PLATFORM;   entity = new Sprite("Resources/Platforms/Madeira/MeiaPlataformaMadeira4.png"); break;
    case PLATFORM_STOPPER:   type = _PLATFORM_STOPPER;   break;
    case GROUND2:            type = GROUND;              entity = new Sprite("Resources/Platforms/Metal/PisoMetal.png"); break;
    case PLATMETAL1:            type = GROUND;              entity = new Sprite("Resources/Platforms/Metal/PlataformaMediaMetal4.png"); break;
    }

    if (entity) {
        width  = entity->Width();
        height = entity->Height();
    }

    if (type == FINISH_PORTAL_FIRE || type == FINISH_PORTAL_WATER || type == FINISH_PORTAL_ANY) {
        entity_2 = new Sprite("Resources/OpenDoor2.png");
    }
    
    Point points[4];
    switch (type) {
    case MOVABLE_BOX:
    case MOVING_PLATFORM_X:
    case MOVING_PLATFORM_Y:
        is_movable = true;
        [[fallthrough]];

    case GROUND:
    case FINISH_PORTAL_FIRE:
    case FINISH_PORTAL_WATER:
    case FINISH_PORTAL_ANY:
    case ROTATING_PLATFORM:
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

    case _PLATFORM_STOPPER:
        BBox(new Rect(-5, -5, 5, 5));
        break;
    }

    MoveTo(posX, posY, posZ);
}

WorldEntity::~WorldEntity()
{
    if (entity)   delete entity;
    if (entity_2) delete entity_2;
}

void WorldEntity::Update()
{
    constexpr float platform_speed = 50;
    constexpr float platform_rotation_speed = 0.5;

    changed_direction = false;
    draw_entity_2 = false;
    switch (type)
    {
    case MOVING_PLATFORM_X:
        Translate((direction_moving ? 1 : -1) * platform_speed * gameTime, 0);
        break;

    case MOVING_PLATFORM_Y:
        Translate(0, (direction_moving ? -1 : 1) * platform_speed * gameTime);
        break;

    case ROTATING_PLATFORM:
        rotation += platform_rotation_speed * gameTime;
        BBox()->RotateTo(rotation);
        break;

    default:
        break;
    }
}

void WorldEntity::OnCollision(Object* obj) {
    switch (type)
    {
    case MOVABLE_BOX:
        if (obj->Type() == GROUND) {
            // Mantém caixa fora do chão (pode colidir apenas no eixo x)
            Translate(-obj->BBox()->mtv_ground.XComponent(), 0);

            // Quando a caixa colide uma vez, não é mais possível move-la
            if (abs(obj->BBox()->mtv_ground.XComponent()) > 1) type = GROUND;
        }
        break;

    case MOVING_PLATFORM_X:
        if (obj->Type() == GROUND || obj->Type() == _PLATFORM_STOPPER) {
            // Mantém caixa fora do chão (pode colidir apenas no eixo x)
            Translate(-obj->BBox()->mtv_water.XComponent(), 0);

            if (!changed_direction) {
                direction_moving = !direction_moving;
                changed_direction = true;
            }
        }
        break;

    case MOVING_PLATFORM_Y:
        if (obj->Type() == GROUND || obj->Type() == _PLATFORM_STOPPER) {
            // Mantém caixa fora do chão (pode colidir apenas no eixo y)
            Translate(0, -obj->BBox()->mtv_water.YComponent());

            if (!changed_direction) {
                direction_moving = !direction_moving;
                changed_direction = true;
            }
        }
        break;
    
    default:
        break;
    }
}
