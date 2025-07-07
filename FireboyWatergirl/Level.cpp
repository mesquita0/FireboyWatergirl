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
    timer = {};

    loadLevel(*this, window, "Level" + std::to_string(level_number) + ".txt");
    Engine::ResetFrameTime();

    FireboyWatergirl::fireboy->LevelNumber(level_number - 1);
    FireboyWatergirl::watergirl->LevelNumber(level_number - 1);

    // inicia com música
    FireboyWatergirl::audio->Frequency(MUSIC, 0.94f);
    FireboyWatergirl::audio->Play(MUSIC);

    timer.Start();
}

void Level::Update()
{
    scene->Update();
    scene->CollisionDetection();

    if (!FireboyWatergirl::fireboy->IsAlive() || !FireboyWatergirl::watergirl->IsAlive())
    {
        FireboyWatergirl::audio->Stop(MUSIC);
        FireboyWatergirl::audio->Play(DIED);
        FireboyWatergirl::GameOverL();
        FireboyWatergirl::fireboy->Reset(level_number-1); 
        FireboyWatergirl::watergirl->Reset(level_number-1); 
    }
    else if (
        (FireboyWatergirl::fireboy->IsReadyNextLevel() 
        && FireboyWatergirl::watergirl->IsReadyNextLevel())
        || window->KeyPress('N'))
    {
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
    font->Draw(window->CenterX() - 25, 15, std::format("{:02}:{:02}", seconds / 60, seconds % 60), Color{1, 1, 1, 1}, 0, 2);

    if (FireboyWatergirl::viewBBox)
        scene->DrawBBox();
}

void Level::Finalize()
{
    delete font;
    scene->Remove(FireboyWatergirl::fireboy, MOVING);
    scene->Remove(FireboyWatergirl::watergirl, MOVING);
    delete scene;
    scene = nullptr;
}
