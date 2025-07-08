#pragma once
#include "Types.h"                                      // tipos espec�ficos da engine
#include "Object.h"                                     // interface de Object
#include "Sprite.h"                                     // desenho de sprites

enum EntityType { 
    GROUND,
    FINISH_PORTAL_FIRE,
    FINISH_PORTAL_WATER,
    THORN,
    MOVABLE_BOX,
    MOVING_PLATFORM_X,
    MOVING_PLATFORM_Y,
    _PLATFORM_STOPPER
};

enum EntityTypeSprite {
    GROUND1,
    PLATWOOD1,
    PLATWOOD2,
    PLATWOOD3,
    PLATWOOD4,
    PLATWOOD5,
    MOVABLE_BOX_WOOD,
    PLATWOOD6,
    FINISH_PORTAL1,
    FINISH_PORTAL2,
    THORN1,
    THORN2,
    THORN3,
    MOVING_PLATFORM_X1,
    MOVING_PLATFORM_Y1,
    PLATFORM_STOPPER
};

class WorldEntity : public Object
{
private:
    Sprite * entity = nullptr;              // sprite da plataforma
    Color color;                            // cor da plataforma
    uint  width = 0;
    uint  height = 0;
    float scale, rotation;
    bool direction_moving, changed_direction, is_movable;

public:
    WorldEntity(float posX, float posY, float posZ,
                EntityTypeSprite platType,
                float scale, float rotation,
                Color tint);                       
    ~WorldEntity();                            

    uint Width() const;
    uint Height() const;

    bool isMovable() const { return is_movable; }

    void Draw();                            // desenho do objeto
    void Update();                          // atualização do objeto
    void OnCollision(Object* obj);
}; 

inline uint WorldEntity::Width() const { return width;  }

inline uint WorldEntity::Height() const { return height;  }

inline void WorldEntity::Draw()
{ if (entity) entity->Draw(x, y, z, scale, rotation, false, color); }
