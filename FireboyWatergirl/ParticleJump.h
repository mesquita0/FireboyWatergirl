#pragma once
#pragma once
#include "Object.h"    
#include "Particles.h"
#include "Types.h"

class ParticleJump : public Object
{
private:
    Particles* p;                       // sistema de part�culas
    int count;
    bool stop;                          // estado da gera��o de fuma�a

public:
    ParticleJump(float pX, float pY);   // posi��o x, y

    ~ParticleJump();                    // destrutor

    int  Size();                        // quantidade de part�culas

    void Update();                      // atualiza��o
    void Draw();                        // desenho
};


inline int ParticleJump::Size()
{
    return p->Size();
}

inline void ParticleJump::Draw()
{
    p->Draw(Layer::MIDDLE, 1.0f);
}
