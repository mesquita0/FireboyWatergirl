#include "Engine.h"
#include "FireboyWatergirl.h"
#include "Home.h"
#include "Home.h"
#include "Player.h"
#include "Background.h"
#include "LevelLoader.h"
#include "WorldEntity.h"

void Home::Init()
{
    scene = new Scene();
    backg = new Background("");

    // inicia com música
    FireboyWatergirl::audio->Play(MENU, true);
}

void Home::Update()
{
    // sai com o pressionar do ESC
    if (window->KeyPress(VK_ESCAPE))
        window->Close();

    // Passar para o level 1
    if (window->KeyPress(VK_RETURN))
    {
        FireboyWatergirl::audio->Stop(MENU);
        FireboyWatergirl::audio->Play(TRANSITION);
        FireboyWatergirl::fireboy->Level(0);
        FireboyWatergirl::watergirl->Level(0);
        FireboyWatergirl::NextLevel();
    }
}

void Home::Draw()
{
    backg->Draw();
}

void Home::Finalize()
{
    delete scene;
    delete backg;

    FireboyWatergirl::audio->Play(MUSIC, true);
}
