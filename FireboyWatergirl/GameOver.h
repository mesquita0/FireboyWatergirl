#pragma once
#include "Font.h"
#include "Game.h"
#include "Sprite.h"

// ------------------------------------------------------------------------------

class GameOver : public Game
{
private:
    Sprite* title = nullptr;               // tela de fim
    Font* font = nullptr;
    float progress_barX;
    float playerPercentage;

public:
    void Init();                    // inicializa��o do n�vel
    void Update();                  // l�gica da tela de fim
    void Draw();                    // desenho da tela
    void Finalize();                // finaliza��o do n�vel
};
