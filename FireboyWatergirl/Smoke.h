#pragma once
#include "Object.h"    
#include "Particles.h"
#include "Types.h"

class Smoke : public Object
{
private:
    Particles* smokeGen;               // sistema de partículas
    bool stop;                          // estado da geração de fumaça

public:
    Smoke(float pX, float pY,           // posição x, y
          float angle);                  // ângulo da fumaça

    ~Smoke();                           // destrutor

    int  Size();                        // quantidade de partículas

    void Update();                      // atualização
    void Draw();                        // desenho
};


inline int Smoke::Size()
{
    return smokeGen->Size();
}

inline void Smoke::Draw()
{
    smokeGen->Draw(Layer::MIDDLE, 1.0f);
}
