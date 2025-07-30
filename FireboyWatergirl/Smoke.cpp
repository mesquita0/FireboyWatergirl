#include "Smoke.h"
#include "FireboyWatergirl.h"

Smoke::Smoke(float pX, float pY, float angle)
{
    // configura gerador de partículas
    Generator fogger;
    fogger.imgFile = "Resources/Smoke.png";    // arquivo de imagem
    fogger.angle = angle;                      // direção da fumaça
    fogger.spread = 20;                        // espalhamento em graus
    fogger.lifetime = 0.25f;                    // tempo de vida em segundos
    fogger.frequency = 0.01f;                 // tempo entre geração de novas partículas
    fogger.percentToDim = 0.87f;               // desaparece após 60% da vida
    fogger.minSpeed = 40.0f;                   // velocidade mínima das partículas
    fogger.maxSpeed = 70.0f;                  // velocidade máxima das partículas
    fogger.minScale = 0;
    fogger.maxScale = 0;

    fogger.color.r = 1.0f;                   // cor da partícula
    fogger.color.g = 1.0f;                   // cor da partícula
    fogger.color.b = 1.0f;                   // cor da partícula
    fogger.color.a = 1.0f;                   // cor da partícula

    // cria sistema de partículas
    smokeGen = new Particles(fogger);
    MoveTo(pX, pY);
    stop = false;
}

Smoke::~Smoke()
{
    delete smokeGen;
}

void Smoke::Update()
{
    smokeGen->Generate(x, y);
    smokeGen->Update(gameTime);
}
