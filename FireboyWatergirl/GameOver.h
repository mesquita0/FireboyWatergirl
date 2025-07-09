#pragma once
#include "Font.h"
#include "Game.h"
#include "Sprite.h"
#include "Button.h"
#include "Scene.h"
#include "Mouse.h"

// ------------------------------------------------------------------------------

class GameOver : public Game
{
private:
    Scene* scene = nullptr;
    Sprite* title = nullptr;               // tela de fim
    Sprite* estrela0 = nullptr;
    Font* font = nullptr;
    float progress_barX;
    float playerPercentage;
    TileSet* tileset = nullptr;    // tileset da anima��o
    Mouse* mouse;
    bool failed = false;
    Sprite* quadrado = nullptr; 
    int seconds;

    Button* play_button, * menu_button;
    //Animation* anim1 = nullptr;  
    //Animation* anim2 = nullptr;
    //Animation* anim3 = nullptr;

public:
    void Init();                    // inicializa��o do n�vel
    void Update();                  // l�gica da tela de fim
    void Draw();                    // desenho da tela
    void Finalize();                // finaliza��o do n�vel
};
