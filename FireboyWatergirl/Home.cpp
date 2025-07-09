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
    backg = new Background("Resources/Background.png",Color{1,1,1,5});
    title = new Sprite("Resources/title.png");
    quadrado1 = new Sprite("Resources/Platforms/teste2.png");
   
    titleY = 180;  

    font = new Font("Resources/Font.png");
    font->Spacing("Resources/FontMetrics.dat");

    mouse = new Mouse();
    scene->Add(mouse, MOVING);

    // Adiciona botões
    callback_function play_sfx = []() { FireboyWatergirl::audio->Play(BUTTON_SELECT); };
    play_button = new Button(480, 384, "PLAY", *font, Color{ 1, 1, 1, 1 }, Color{ 1, 1, 0, 1 }, 2, play_sfx);
    quit_button = new Button(480, 384 + 100, "QUIT", *font, Color{1, 1, 1, 1}, Color{1, 1, 0, 1}, 2, play_sfx);
    scene->Add(play_button, STATIC);
    scene->Add(quit_button, STATIC);

    // Adiciona players
    FireboyWatergirl::fireboy->Reset();
    FireboyWatergirl::watergirl->Reset();
    scene->Add(FireboyWatergirl::fireboy, STATIC);
    scene->Add(FireboyWatergirl::watergirl, STATIC);
    FireboyWatergirl::fireboy->MoveTo(window->CenterX() - 30, window->CenterY() - 50);
    FireboyWatergirl::watergirl->MoveTo(window->CenterX() + 30, window->CenterY() - 55);
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
        FireboyWatergirl::audio->Play(LEVEL_START);
        FireboyWatergirl::fireboy->LevelNumber(0);
        FireboyWatergirl::watergirl->LevelNumber(0);
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
    title->Draw(window->CenterX(), titleY, Layer::FRONT);
    quadrado1->Draw(window->CenterX(), window->CenterY(), Layer::MIDDLE);
    quadrado1->Draw(window->CenterX(), window->CenterY() + 80, Layer::MIDDLE);


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
}
