#include "Level.h"
#include "FireboyWatergirl.h"
#include "Engine.h"
#include "LevelLoader.h"
#include "Player.h"
#include "Background.h"
#include "Font.h"
#include "WorldEntity.h"
#include <format>
#include <string>

using std::string;

Scene * Level::scene = nullptr;

Level::Level(int level_number) : level_number(level_number) {}

constexpr int game_width  = 960;
constexpr int game_height = 768;

void Level::Init()
{
    if (scene) return;
    
    scene = new Scene();
    backg = new Background("");
    font  = new Font("Resources/Font.png");
    font->Spacing("Resources/FontMetrics.dat");
    time_frame = new Sprite("Resources/TimeFrame.png");
    timer = {};

    loadLevel(*this, window, "Level" + std::to_string(level_number) + ".txt");
    if (level_number == 2) is_run = true;
    Engine::ResetFrameTime();

    FireboyWatergirl::fireboy->LevelNumber(level_number - 1);
    FireboyWatergirl::watergirl->LevelNumber(level_number - 1);

    // inicia com música
    FireboyWatergirl::audio->Frequency(MUSIC, 0.94f);
    FireboyWatergirl::audio->Play(MUSIC, true);
    timer.Start();

    // ----------------------
    // inicializa a viewport
    // ----------------------

    // calcula posição para manter viewport centralizada
    float difx = (game_width  * FireboyWatergirl::zoom - window->Width()) / 2.0f;
    float dify = (game_height * FireboyWatergirl::zoom - window->Height()) / 2.0f;

    // inicializa viewport para o centro do mundo
    viewport.left = 0.0f + difx;
    viewport.right = viewport.left + window->Width();
    viewport.top = 0.0f + dify;
    viewport.bottom = viewport.top + window->Height();
}

void Level::Update()
{
    scene->Update();
    scene->CollisionDetection();

    bool fireboy_ready   = FireboyWatergirl::fireboy->IsReadyNextLevel() && FireboyWatergirl::fireboy->IsStill();
    bool watergirl_ready = FireboyWatergirl::watergirl->IsReadyNextLevel() && FireboyWatergirl::watergirl->IsStill();

    if (!FireboyWatergirl::fireboy->IsAlive() || !FireboyWatergirl::watergirl->IsAlive())
    {
        did_fail = true;
        FireboyWatergirl::audio->Stop(MUSIC);
        FireboyWatergirl::audio->Play(DIED);
        FireboyWatergirl::NextLevel();
        FireboyWatergirl::fireboy->Reset(level_number-1); 
        FireboyWatergirl::watergirl->Reset(level_number-1); 
    }
    else if ((fireboy_ready && watergirl_ready) || (is_run && (fireboy_ready || watergirl_ready)) || window->KeyPress('N'))
    {
        did_fail = false;
        if (is_run) {
            did_fireboy_win = fireboy_ready;
        }

        FireboyWatergirl::audio->Stop(MUSIC);
        FireboyWatergirl::fireboy->Reset(level_number-1);
        FireboyWatergirl::watergirl->Reset(level_number-1); 
        FireboyWatergirl::NextLevel();
    } 

    // --------------------
    // atualiza a viewport
    // --------------------

    viewport.left = FireboyWatergirl::fireboy->X() - window->CenterX()   * ( 1 / FireboyWatergirl::zoom);
    viewport.right = FireboyWatergirl::fireboy->X() + window->CenterX()  * ( 1 / FireboyWatergirl::zoom);
    viewport.top = FireboyWatergirl::fireboy->Y() - window->CenterY()    * ( 1 / FireboyWatergirl::zoom);
    viewport.bottom = FireboyWatergirl::fireboy->Y() + window->CenterY() * ( 1 / FireboyWatergirl::zoom);

    if (viewport.left < 0)
    {
        viewport.left = 0;
        viewport.right = window->Width() * (1 / FireboyWatergirl::zoom);
    }
    else if (viewport.right > game->Width())
    {
        viewport.left = game->Width() - window->Width() * (1 / FireboyWatergirl::zoom);
        viewport.right = game->Width();
    }

    if (viewport.top < 0)
    {
        viewport.top = 0;
        viewport.bottom = window->Height() * (1 / FireboyWatergirl::zoom);
    }
    else if (viewport.bottom > game->Height())
    {
        viewport.top = game->Height() - window->Height() * (1 / FireboyWatergirl::zoom);
        viewport.bottom = game->Height();
    }
}

void Level::Draw()
{
    backg->Draw();
    scene->Draw();

    float px = (viewport.right + viewport.left) / 2.0f;
    float py = viewport.top + 15;

    // Tempo do level
    int seconds = timer.Elapsed();
    time_frame->Draw(px + 15, py, Layer::FRONT, 0.21);
    font->Draw(px, py, std::format("{:02}:{:02}", seconds / 60, seconds % 60), Color{0, 0, 0, 1}, 0, 2);

    if (FireboyWatergirl::viewBBox)
        scene->DrawBBox();
}

void Level::Finalize()
{
    delete time_frame;
    delete font;
    scene->Remove(FireboyWatergirl::fireboy, MOVING);
    scene->Remove(FireboyWatergirl::watergirl, MOVING);
    delete scene;
    scene = nullptr;
}
