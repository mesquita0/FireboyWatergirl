#pragma once
#include "Game.h"
#include "Scene.h"
#include "Player.h"
#include "Background.h"

class Level : public Game
{
private:
    int level_number;

public:
    Background* backg = nullptr;   // pano de fundo
    static Scene* scene;           // cena do nível
    int num_ground_blocks = 0;

    Level(int level_number);

    void Init();                    // inicialização do nível
    void Update();                  // atualiza lógica do jogo
    void Draw();                    // desenha jogo
    void Finalize();                // finalização do nível

    int levelNumber();
};

inline int Level::levelNumber()
{ return level_number; }
