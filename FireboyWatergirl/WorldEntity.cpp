#include "WorldEntity.h"
#include "FireboyWatergirl.h"

WorldEntity::WorldEntity(float posX, float posY, EntityTypeSprite platType, Color tint) : color(tint)
{
    switch (platType)
    {
    case GROUND1:         type = GROUND;              entity = new Sprite("Resources/Block/GroundSquare.png"); break;
    case THORN1:          type = THORN;               entity = new Sprite("Resources/Thorn/Spike01.png"); break;
    case THORN2:          type = THORN;               entity = new Sprite("Resources/Thorn/Spike02.png"); break;
    case INVERTED_THORN1: type = INVERTED_THORN;      entity = new Sprite("Resources/Thorn/InvertedSpike01.png"); break;
    case INVERTED_THORN2: type = INVERTED_THORN;      entity = new Sprite("Resources/Thorn/InvertedSpike02.png"); break;
    case SMALL_THORN1:    type = THORN;               entity = new Sprite("Resources/Thorn/SmallSpike01.png"); break;
    case SMALL_THORN2:    type = THORN;               entity = new Sprite("Resources/Thorn/SmallSpike02.png"); break;
    case THORN_PIT1:      type = THORN_PIT;           entity = new Sprite("Resources/Thorn/ThornPit01.png"); break;
    case THORN_PIT2:      type = THORN_PIT;           entity = new Sprite("Resources/Thorn/ThornPit02.png"); break;
    case THORN_PIT3:      type = THORN_PIT;           entity = new Sprite("Resources/Thorn/ThornPit03.png"); break;
    case BLOCK_GRID:      type = BLOCK;               entity = new Sprite("Resources/Block/GridBlock01.png"); break;
    case BLOCK1:          type = BLOCK;               entity = new Sprite("Resources/Block/Block01.png"); break;
    case BLOCK2:          type = BLOCK;               entity = new Sprite("Resources/Block/Block02.png"); break;
    case BLOCK3:          type = BLOCK;               entity = new Sprite("Resources/Block/Block03.png"); break;
    case BLOCK4:          type = BLOCK;               entity = new Sprite("Resources/Block/Block04.png"); break;
    case BLOCK5:          type = BLOCK;               entity = new Sprite("Resources/Block/Block05.png"); break;
    case JUMP_PAD1:       type = JUMP_PAD;            entity = new Sprite("Resources/Transporter/YellowJumpPad.png"); break;
    case JUMP_RING1:      type = JUMP_RING;           entity = new Sprite("Resources/Transporter/YellowJumpRing.png"); break;
    case GRAVITY_PORTAL1: type = GRAVITY_PORTAL_UP;   entity = new Sprite("Resources/Portal/GravityPortalUp.png"); break;
    case GRAVITY_PORTAL2: type = GRAVITY_PORTAL_DOWN; entity = new Sprite("Resources/Portal/GravityPortalDown.png"); break;
    case STOP_CAMERA:     type = _STOP_CAMERA; break;
    case MOVE_CAMERA:     type = _MOVE_CAMERA; break;
    case BLOCK_SIDE:      type = _BLOCK_SIDE; break;
    case FINISH:          type = _FINISH; break;
    }

    if (entity) {
        width  = entity->Width();
        height = entity->Height();
    }
    
    Point points[3];
    switch (type) {
    case GROUND:
    case BLOCK:
    case THORN_PIT:
    case JUMP_PAD:
    case JUMP_RING:
    case GRAVITY_PORTAL_UP:
    case GRAVITY_PORTAL_DOWN:
        BBox(new Rect(-entity->Width() / 2.0f,
            -entity->Height() / 2.0f,
            entity->Width() / 2.0f,
            entity->Height() / 2.0f));
        break;

    case _BLOCK_SIDE:
        points[0] = { -34 / 2.0f, -34 / 2.0f + 7 };
        points[1] = { -34 / 2.0f, 34 / 2.0f - 10 };
        BBox(new Poly(points, 2));
        break;

    case THORN:
        points[0] = { -entity->Width() / 2.0f, entity->Height() / 2.0f };
        points[1] = { entity->Width() / 2.0f, entity->Height() / 2.0f };
        points[2] = { 0.0, -entity->Height() / 2.0f };
        BBox(new Poly(points, 3));
        break;

    case INVERTED_THORN:
        points[0] = { -entity->Width() / 2.0f, -entity->Height() / 2.0f };
        points[1] = { entity->Width() / 2.0f, -entity->Height() / 2.0f };
        points[2] = { 0.0, entity->Height() / 2.0f };
        BBox(new Poly(points, 3));
        break;

    case _STOP_CAMERA:
    case _MOVE_CAMERA:
    case _FINISH:
        BBox(new Rect(-10,
            -posY,
            10,
            window->Height() - posY));
        break;
    }

    MoveTo(posX, posY, Layer::MIDDLE);
}

WorldEntity::~WorldEntity()
{
    delete entity;
}

void WorldEntity::Update()
{
    
}
