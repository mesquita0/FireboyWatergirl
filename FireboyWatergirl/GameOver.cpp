#include "Engine.h"
#include "GameOver.h"
#include "FireboyWatergirl.h"
#include "Home.h"
#include "Mouse.h"
#include <format>

void GameOver::Init()
{
    scene = new Scene();
    title = new Sprite("Resources/Platforms/backback.png");
    font = new Font("Resources/Font.png");
    font->Spacing("Resources/FontMetrics.dat");
    quadrado = new Sprite("Resources/Platforms/quadrado.png");

    mouse = new Mouse();
    scene->Add(mouse, MOVING);

    int level_number = FireboyWatergirl::fireboy->LevelNumber();
    seconds = static_cast<Level*>(FireboyWatergirl::last_level)->timer.Elapsed();

    //if (level_number == failed) {
    //    title = new Sprite("Resources/Platforms/backback.png");
    //} 
    if (!FireboyWatergirl::fireboy->IsAlive() || !FireboyWatergirl::watergirl->IsAlive()) {
        title = new Sprite("Resources/Platforms/backback.png");
    }
    else if (level_number == 0) {
        if (seconds <= 30) {
            title = new Sprite("Resources/back3.png");
        }
        else if (seconds < 50) {
            title = new Sprite("Resources/back2.png");
        }
        else if (seconds > 50) {
            title = new Sprite("Resources/back1.png");
        }
    }
    else if (level_number == 1) {
        if (seconds <= 30) {
            title = new Sprite("Resources/back3.png");
        }
        else if (seconds < 50) {
            title = new Sprite("Resources/back2.png");
        }
        else if (seconds > 50) {
            title = new Sprite("Resources/back1.png");
        }

        if (static_cast<Level*>(FireboyWatergirl::last_level)->didFireboyWin()) {
            scene->Add(FireboyWatergirl::fireboy, STATIC);
            FireboyWatergirl::watergirl->MoveTo(window->CenterX() + 30, 400); 
        }
        else {
            scene->Add(FireboyWatergirl::watergirl, STATIC);
            FireboyWatergirl::fireboy->MoveTo(window->CenterX() - 30, 400);
        }
    }
        
    // Adiciona botões
    callback_function play_sfx = []() { FireboyWatergirl::audio->Play(BUTTON_SELECT); };
    failed = static_cast<Level*>(FireboyWatergirl::last_level)->failed();
    if (level_number != 1 || failed) {
        play_button = new Button(window->CenterX() - 173, window->CenterY() + 78, failed ? "Try Again" : "Continue", *font, Color{ 1, 1, 1, 1 }, Color{ 1, 1, 0, 1 }, 1.75, play_sfx);
        scene->Add(play_button, STATIC);
        menu_button = new Button(window->CenterX() + 83, window->CenterY() + 78, "Main Menu", *font, Color{ 1, 1, 1, 1 }, Color{ 1, 1, 0, 1 }, 1.65, play_sfx);
    }
    else {
        menu_button = new Button(window->CenterX() - 45, window->CenterY() + 78, "Main Menu", *font, Color{ 1, 1, 1, 1 }, Color{ 1, 1, 0, 1 }, 1.65, play_sfx);
    }

    scene->Add(menu_button, STATIC);

    FireboyWatergirl::fireboy->disableControls();
    FireboyWatergirl::watergirl->disableControls();
}

void GameOver::Update()
{
    scene->Update();
    scene->CollisionDetection();

    // Passar para o level 1
    if (play_button && scene->Collision(mouse, play_button) && mouse->Clicked())
    {
        FireboyWatergirl::fireboy->Reset();
        FireboyWatergirl::watergirl->Reset();
        if (!failed) FireboyWatergirl::NextLevel();
        else FireboyWatergirl::LastLevel();
    }
    else if ((scene->Collision(mouse, menu_button) && mouse->Clicked()) || window->KeyPress(VK_ESCAPE)) {
        FireboyWatergirl::fireboy->Reset();
        FireboyWatergirl::watergirl->Reset();
        FireboyWatergirl::HomeLevel();
    }
}

void GameOver::Draw()
{
    scene->Draw();
    title->Draw(window->CenterX(), window->CenterY(), Layer::BACK); 
    
    font->Draw(window->CenterX() - 25, 100, std::format("{:02}:{:02}", seconds / 60, seconds % 60), Color{ 0, 0, 0, 1 }, 0, 2);

    if (!static_cast<Level*>(FireboyWatergirl::last_level)->is_run || static_cast<Level*>(FireboyWatergirl::last_level)->failed()) {
        quadrado->Draw(window->CenterX() - 130, window->CenterY() + 80, Layer::MIDDLE);
        quadrado->Draw(window->CenterX() + 130, window->CenterY() + 80, Layer::MIDDLE);
    }
    else {
        quadrado->Draw(window->CenterX(), window->CenterY() + 80, Layer::MIDDLE);
    }

    if (FireboyWatergirl::viewBBox)
        scene->DrawBBox();
}

void GameOver::Finalize()
{
    scene->Remove(FireboyWatergirl::fireboy, STATIC);
    scene->Remove(FireboyWatergirl::watergirl, STATIC);

    delete scene;
    delete font;
    delete title;
}
