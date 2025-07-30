#pragma once
#include "Object.h"    
#include "Particles.h"
#include "Types.h"

class Smoke : public Object
{
private:
    Particles* smokeGen;               // sistema de part�culas
    bool stop;                          // estado da gera��o de fuma�a

public:
    Smoke(float pX, float pY,           // posi��o x, y
          float angle);                  // �ngulo da fuma�a

    ~Smoke();                           // destrutor

    int  Size();                        // quantidade de part�culas

    void Update();                      // atualiza��o
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
