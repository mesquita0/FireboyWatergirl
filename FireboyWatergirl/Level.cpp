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
        FireboyWatergirl::audio->Stop(MUSIC);
        FireboyWatergirl::fireboy->Reset(level_number-1);
        FireboyWatergirl::watergirl->Reset(level_number-1); 
        FireboyWatergirl::NextLevel();
    } 
}

void Level::Draw()
{
    backg->Draw();
    scene->Draw();

    // Tempo do level
    int seconds = timer.Elapsed();
    time_frame->Draw(window->CenterX() - 10, 15, Layer::FRONT, 0.21);
    font->Draw(window->CenterX() - 25, 15, std::format("{:02}:{:02}", seconds / 60, seconds % 60), Color{0, 0, 0, 1}, 0, 2);

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
