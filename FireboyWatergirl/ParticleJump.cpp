#include "ParticleJump.h"
#include "Level.h"
#include "FireboyWatergirl.h"

ParticleJump::ParticleJump(float pX, float pY)
{
    Generator gen;
    gen.imgFile = "Resources/particula.png";
    gen.angle = 90.0f;
    gen.spread = 180.0f;
    gen.lifetime = 0.2f;
    gen.frequency = 0.0f;
    gen.percentToDim = 0.1f;
    gen.minSpeed = 20.0f;
    gen.maxSpeed = 150.0f;
    gen.minScale = 0.8f;
    gen.maxScale = 1.5f;
    gen.color = { 1.0f, 1.0f, 1.0f, 0.8f };

    // cria sistema de partículas
    p = new Particles(gen);
    MoveTo(pX, pY, 7);
    p->Generate(pX, pY);
    stop = false;
}

ParticleJump::~ParticleJump()
{
    delete p;
}

void ParticleJump::Update()
{ 
    if (++count > 15) {
        count = 0;
        stop = true;
    }


    if (stop)
    {
        if (p->Inactive())
            static_cast<Level*>(FireboyWatergirl::current_level)->scene->Delete();
    }
    else
    {
        p->Generate(x, y);
    }

    p->Update(gameTime);
}
