#pragma once
#include "Game.h"
#include "Scene.h"
#include "Player.h"
#include "Background.h"
#include "Font.h"
#include "Timer.h"

class Level : public Game
{
private:
    int level_number;
    

public:
    static Scene* scene;           // cena do n�vel
    Background * backg = nullptr;  // pano de fundo
    Sprite * time_frame = nullptr;
    Font * font;
    int num_ground_blocks = 0;
    bool is_run = false;

    Level(int level_number);

    void Init();                    // inicializa��o do n�vel
    void Update();                  // atualiza l�gica do jogo
    void Draw();                    // desenha jogo
    void Finalize();                // finaliza��o do n�vel

    int levelNumber();
    Timer timer; 
}; 

inline int Level::levelNumber()
{ return level_number; } 
