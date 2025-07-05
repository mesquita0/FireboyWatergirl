#pragma once
#include "Game.h"
#include "Button.h"
#include "Sprite.h"
#include "TileSet.h"
#include "Animation.h"
#include "Font.h" 
#include "Scene.h"
#include "Player.h"
#include "Background.h"
#include "Level.h"
#include "Mouse.h"

class Home : public Game
{
private:
    Scene * scene = nullptr;
    Background * backg = nullptr;
    Font * font = nullptr;

    Mouse * mouse;
    Button * play_button;
    Button * quit_button;

public:
    Home() = default;

    void Init();                    // inicia nível
    void Update();                  // atualiza lógica do jogo
    void Draw();                    // desenha jogo
    void Finalize();                // finaliza nível
};
