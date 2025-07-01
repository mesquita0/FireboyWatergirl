#include "WorldEntity.h"
#include "FireboyWatergirl.h"

WorldEntity::WorldEntity(float posX, float posY, EntityTypeSprite platType, float scale, float rotation, Color tint) 
    : scale(scale), rotation(rotation), color(tint)
{
    switch (platType)
    {
    case GROUND1: type = GROUND; entity = new Sprite("Resources/LongGray.png"); break;
    }

    if (entity) {
        width  = entity->Width();
        height = entity->Height();
    }
    
    Point points[3];
    switch (type) {
    case GROUND:
        BBox(new Rect(-entity->Width() / 2.0f,
            -entity->Height() / 2.0f,
            entity->Width() / 2.0f,
            entity->Height() / 2.0f));
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
