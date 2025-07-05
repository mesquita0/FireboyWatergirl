#pragma once
#include "Game.h"
#include "Sprite.h"
#include "TileSet.h"
#include "Animation.h"
#include "Game.h" 
#include "Scene.h"
#include "Player.h"
#include "Background.h"
#include "Level.h"

class Home : public Game
{
private:
    Scene * scene = nullptr;
    Sprite* backg = nullptr;

public:
    Home() = default;

    void Init();                    // inicia n�vel
    void Update();                  // atualiza l�gica do jogo
    void Draw();                    // desenha jogo
    void Finalize();                // finaliza n�vel
};
