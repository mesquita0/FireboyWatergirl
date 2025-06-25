#include "Level.h"
#include "FireboyWatergirl.h"
#include "LevelLoader.h"
#include "Player.h"
#include "Background.h"
#include <string>

using std::string;

Scene * Level::scene = nullptr;

Level::Level(int level_number) : level_number(level_number) {}

void Level::Init()
{
    if (scene) return;
        
    // cria gerenciador de cena
    scene = new Scene();

    //loadLevel(*this, window, "Level" + std::to_string(level_number) + ".txt");
    scene->Add(FireboyWatergirl::fireboy, MOVING);
    //scene->Add(FireboyWatergirl::watergirl, MOVING);

    FireboyWatergirl::fireboy->Level(level_number - 1);
    FireboyWatergirl::watergirl->Level(level_number - 1);
    FireboyWatergirl::fireboy->MoveTo(window->CenterX(), window->CenterY() + 100);

    // inicia com m�sica
    FireboyWatergirl::audio->Frequency(MUSIC, 0.94f);
    FireboyWatergirl::audio->Play(MUSIC);
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
    else if (FireboyWatergirl::fireboy->Level() != level_number - 1 || window->KeyPress('N'))
    {
        FireboyWatergirl::fireboy->Reset(level_number);
        FireboyWatergirl::watergirl->Reset(level_number-1); 
        FireboyWatergirl::NextLevel();
    } 
}

void Level::Draw()
{
    //backg->Draw();
    scene->Draw();

    if (FireboyWatergirl::viewBBox)
        scene->DrawBBox();
}

void Level::Finalize()
{
    scene->Remove(FireboyWatergirl::fireboy, MOVING);
    scene->Remove(FireboyWatergirl::watergirl, MOVING);
    delete scene;
    scene = nullptr;
}
