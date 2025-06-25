#pragma once
#include "Types.h"                                      // tipos específicos da engine
#include "Object.h"                                     // interface de Object
#include "Sprite.h"                                     // desenho de sprites

enum EntityType { 
    GROUND, 
    THORN, 
    INVERTED_THORN, 
    THORN_PIT, 
    BLOCK, 
    _BLOCK_SIDE, 
    JUMP_PAD, 
    JUMP_RING, 
    GRAVITY_PORTAL_UP, 
    GRAVITY_PORTAL_DOWN, 
    _STOP_CAMERA, 
    _MOVE_CAMERA,
    _FINISH 
};

enum EntityTypeSprite {
    GROUND1,
    THORN1,
    THORN2,
    INVERTED_THORN1,
    INVERTED_THORN2,
    SMALL_THORN1,
    SMALL_THORN2,
    THORN_PIT1,
    THORN_PIT2,
    THORN_PIT3,
    BLOCK_GRID,
    BLOCK1,
    BLOCK2,
    BLOCK3,
    BLOCK4,
    BLOCK5,
    JUMP_PAD1,
    JUMP_RING1,
    GRAVITY_PORTAL1,
    GRAVITY_PORTAL2,
    BLOCK_SIDE,
    STOP_CAMERA,
    MOVE_CAMERA,
    FINISH
};

class WorldEntity : public Object
{
private:
    Sprite * entity = nullptr;            // sprite da plataforma
    Color color;                            // cor da plataforma
    uint  width = 0;
    uint  height = 0;

public:
    WorldEntity(float posX, float posY, 
                EntityTypeSprite platType,
                Color tint);                       
    ~WorldEntity();                            

    uint Width() const;
    uint Height() const;

    void Update();                          // atualização do objeto
    void Draw();                            // desenho do objeto
}; 

inline uint WorldEntity::Width() const { return width;  }

inline uint WorldEntity::Height() const { return height;  }

inline void WorldEntity::Draw()
{ if (entity) entity->Draw(x, y, z, 1.0f, 0.0f, false, color); }
