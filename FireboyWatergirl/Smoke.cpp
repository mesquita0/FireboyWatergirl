#include "Smoke.h"
#include "FireboyWatergirl.h"

Smoke::Smoke(float pX, float pY, float angle)
{
    // configura gerador de part�culas
    Generator fogger;
    fogger.imgFile = "Resources/Smoke.png";    // arquivo de imagem
    fogger.angle = angle;                      // dire��o da fuma�a
    fogger.spread = 20;                        // espalhamento em graus
    fogger.lifetime = 0.25f;                    // tempo de vida em segundos
    fogger.frequency = 0.01f;                 // tempo entre gera��o de novas part�culas
    fogger.percentToDim = 0.87f;               // desaparece ap�s 60% da vida
    fogger.minSpeed = 40.0f;                   // velocidade m�nima das part�culas
    fogger.maxSpeed = 70.0f;                  // velocidade m�xima das part�culas
    fogger.minScale = 0;
    fogger.maxScale = 0;

    fogger.color.r = 1.0f;                   // cor da part�cula
    fogger.color.g = 1.0f;                   // cor da part�cula
    fogger.color.b = 1.0f;                   // cor da part�cula
    fogger.color.a = 1.0f;                   // cor da part�cula

    // cria sistema de part�culas
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
