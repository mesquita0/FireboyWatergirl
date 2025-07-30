#pragma once
#pragma once
#include "Object.h"    
#include "Particles.h"
#include "Types.h"

class ParticleJump : public Object
{
private:
    Particles* p;                       // sistema de partículas
    int count;
    bool stop;                          // estado da geração de fumaça

public:
    ParticleJump(float pX, float pY);   // posição x, y

    ~ParticleJump();                    // destrutor

    int  Size();                        // quantidade de partículas

    void Update();                      // atualização
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
