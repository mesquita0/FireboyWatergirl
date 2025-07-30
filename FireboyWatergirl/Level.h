#pragma once
#include "Game.h"
#include "Scene.h"
#include "Player.h"
#include "Background.h"
#include "Font.h"
#include "Timer.h"
<<<<<<< HEAD
#include "Particles.h" 
#include <list>       
using std::list;       
=======
#include "Smoke.h"
>>>>>>> eefc5bd7db42eff75791b53485566f1cdbef5e6b

class Level : public Game
{
private:
    int level_number;
    list<Particles*> particleSystems; 
    

public:
    static Scene* scene;           // cena do nível
    Background * backg = nullptr;  // pano de fundo
    Sprite * time_frame = nullptr;
    Font * font;
    Smoke * smoke;
    int num_ground_blocks = 0;
    bool is_run = false, did_fail = false, did_fireboy_win = false;
    float height, width;
    void AddParticleSystem(float x, float y);

    Level(int level_number);

    bool failed();
    bool didFireboyWin();

    void Init();                    // inicialização do nível
    void Update();                  // atualiza lógica do jogo
    void Draw();                    // desenha jogo
    void Finalize();                // finalização do nível

    float Width();
    float Height();

    int levelNumber();
    Timer timer; 
}; 

inline bool Level::failed()
{ return did_fail; }

inline bool Level::didFireboyWin()
{ return did_fireboy_win; }

inline int Level::levelNumber()
{ return level_number; } 

inline float Level::Width()
{ return width; }

inline float Level::Height()
{ return height; }
