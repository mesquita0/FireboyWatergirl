#include "WorldEntity.h"
#include "FireboyWatergirl.h"

WorldEntity::WorldEntity(float posX, float posY, EntityTypeSprite platType, float scale, float rotation, Color tint) 
    : scale(scale), rotation(rotation), color(tint)
{
    switch (platType)
    {
    case GROUND1:        type = GROUND;              entity = new Sprite("Resources/LongGray.png"); break;
    case FINISH_PORTAL1: type = FINISH_PORTAL_FIRE;  entity = new Sprite("Resources/FireDoor.png"); break;
    case FINISH_PORTAL2: type = FINISH_PORTAL_WATER; entity = new Sprite("Resources/WaterDoor.png"); break;
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
        points[0] = { -entity->Width() / 2.0f, -entity->Height() / 2.0f };
        points[1] = {  entity->Width() / 2.0f, -entity->Height() / 2.0f };
        points[3] = { -entity->Width() / 2.0f,  entity->Height() / 2.0f };
        points[2] = {  entity->Width() / 2.0f,  entity->Height() / 2.0f };

        BBox(new Poly(points, 4));

        BBox()->RotateTo(rotation);
        BBox()->ScaleTo(scale);

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
