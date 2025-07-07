#pragma once
#include "Types.h"                                      // tipos espec�ficos da engine
#include "Object.h"                                     // interface de Object
#include "Sprite.h"                                     // desenho de sprites

enum EntityType { 
    GROUND,
    FINISH_PORTAL_FIRE,
    FINISH_PORTAL_WATER
};

enum EntityTypeSprite {
    GROUND1,
    PLATWOOD1,
    PLATWOOD2,
    PLATWOOD3,
    PLATWOOD4,
    PLATWOOD5,
    PLATWOOD6,
    FINISH_PORTAL1,
    FINISH_PORTAL2
};

class WorldEntity : public Object
{
private:
    Sprite * entity = nullptr;              // sprite da plataforma
    Color color;                            // cor da plataforma
    uint  width = 0;
    uint  height = 0;
    float scale, rotation;

public:
    WorldEntity(float posX, float posY, float posZ,
                EntityTypeSprite platType,
                float scale, float rotation,
                Color tint);                       
    ~WorldEntity();                            

    uint Width() const;
    uint Height() const;

    void Update();                          // atualiza��o do objeto
    void Draw();                            // desenho do objeto
}; 

inline uint WorldEntity::Width() const { return width;  }

inline uint WorldEntity::Height() const { return height;  }

inline void WorldEntity::Draw()
{ if (entity) entity->Draw(x, y, z, scale, rotation, false, color); }
