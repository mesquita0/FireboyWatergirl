#include "Home.h"
#include "Engine.h"
#include "Button.h"
#include "FireboyWatergirl.h"
#include "Font.h"
#include "Player.h"
#include "Background.h"
#include "LevelLoader.h"
#include "WorldEntity.h"

void Home::Init()
{
    scene = new Scene();
    backg = new Background("");

    font = new Font("Resources/Font.png");
    font->Spacing(60);

    mouse = new Mouse();
    scene->Add(mouse, MOVING);

    // Adiciona botões
    play_button = new Button(window->CenterX(), window->CenterY(), "PLAY", *font, Color{1, 1, 1, 1}, Color{1, 1, 0, 1}, 0.4f);
    quit_button = new Button(window->CenterX(), window->CenterY() + 100, "QUIT", *font, Color{1, 1, 1, 1}, Color{1, 1, 0, 1}, 0.4f);
    scene->Add(play_button, STATIC);
    scene->Add(quit_button, STATIC);

    // Adiciona players
    FireboyWatergirl::fireboy->Reset();
    FireboyWatergirl::watergirl->Reset();
    scene->Add(FireboyWatergirl::fireboy, STATIC);
    scene->Add(FireboyWatergirl::watergirl, STATIC);
    FireboyWatergirl::fireboy->MoveTo(200, 750);
    FireboyWatergirl::watergirl->MoveTo(800, 750);
    FireboyWatergirl::fireboy->disableControls();
    FireboyWatergirl::watergirl->disableControls();

    // inicia com música
    FireboyWatergirl::audio->Play(MENU, true);
}

void Home::Update()
{
    scene->Update();
    scene->CollisionDetection();

     // Passar para o level 1
    if (scene->Collision(mouse, play_button) && mouse->Clicked())
    {
        FireboyWatergirl::audio->Stop(MENU);
        FireboyWatergirl::audio->Play(TRANSITION);
        FireboyWatergirl::fireboy->Level(0);
        FireboyWatergirl::watergirl->Level(0);
        FireboyWatergirl::NextLevel();
    }
    else if ((scene->Collision(mouse, quit_button) && mouse->Clicked()) || window->KeyPress(VK_ESCAPE)) {
        window->Close();
    }
}

void Home::Draw()
{
    backg->Draw();
    scene->Draw();

    if (FireboyWatergirl::viewBBox)
        scene->DrawBBox();
}

void Home::Finalize()
{
    scene->Remove(FireboyWatergirl::fireboy, STATIC);
    scene->Remove(FireboyWatergirl::watergirl, STATIC);

    delete scene;
    delete backg;
    delete font;
  
    FireboyWatergirl::audio->Play(MUSIC, true);
}
