#pragma once
#include "Types.h"                                      // tipos espec�ficos da engine
#include "Object.h"                                     // interface de Object
#include "Sprite.h"                                     // desenho de sprites
#include "FireboyWatergirl.h"                                     // desenho de sprites
#include "TileSet.h" 

enum EntityType { 
    GROUND,
    FINISH_PORTAL_FIRE,
    FINISH_PORTAL_WATER,
    THORN,
    MOVABLE_BOX,
    MOVING_PLATFORM_X,
    MOVING_PLATFORM_Y,
    _PLATFORM_STOPPER,
    FINISH_PORTAL_ANY,
    ROTATING_PLATFORM,
    _WATER,
    _LAVA,
    _POISON,
    ENEMY,
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
    PLATFORM_STOPPER,
    FINISH_PORTAL_3,
    ROTATING_PLATFORM1,
    GROUND2,
    PLATMETAL1,
    PLATMETAL2,
    BLOCOMETAL,
    RAMPAMETAL, 
    WATER,
    LAVA,
    POISON,
};

class WorldEntity : public Object
{
private:
    Sprite * entity = nullptr, * entity_2 = nullptr; // sprite da plataforma
    Animation * anim = nullptr;                      // animação da plataforma
    Color color;                                     // cor da plataforma
    uint  width = 0;
    uint  height = 0;
    float scale, rotation;
    bool direction_moving, changed_direction, is_movable, draw_entity_2;

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
    void ToggleEnity();
}; 

inline uint WorldEntity::Width() const { return width;  }

inline uint WorldEntity::Height() const { return height;  }

inline void WorldEntity::Draw()
{ 
    Sprite* current_entity = draw_entity_2 ? entity_2 : entity;
    if (current_entity) {
        current_entity->Draw(
            x,
            y,
            z,
            scale,
            rotation,
            false,
            color
        );
    }
    else if (anim) {
        anim->Draw(
            x,
            y,
            z,
            scale,
            rotation,
            false,
            color
        );
    }
}

inline void WorldEntity::ToggleEnity()
{ draw_entity_2 = true; }
