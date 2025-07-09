#pragma once
#include "Audio.h"
#include "Controller.h"
#include "Game.h"
#include "Player.h"
#include "Resources.h"
#include <vector>

enum Sounds { MENU, MUSIC, DIED, FB_JUMP, WG_JUMP, GAMEOVER, LEVEL_START, BUTTON_SELECT };

class FireboyWatergirl : public Game
{
private:
    static int level_index;

public:
    static Player * fireboy;
    static Player * watergirl;
    static Audio * audio;           // sistema de �udio
    static Game * current_level;    // n�vel atual do jogo
    static bool viewBBox;           // estado da bounding box
    static int game_speed;

    void Init();                    // inicializa jogo
    void Update();                  // atualiza l�gica do jogo
    void Draw();                    // desenha jogo
    void Finalize();                // finaliza jogo

    static void NextLevel();        // muda para pr�ximo n�vel do jogo
    static void GameOverL();

    static std::vector<Game*> levels;
};
