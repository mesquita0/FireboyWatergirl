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

    mouse = new Mouse();
    scene->Add(mouse, MOVING);

    int level_number = FireboyWatergirl::fireboy->LevelNumber();
    int time = static_cast<Level*>(FireboyWatergirl::last_level)->timer.Elapsed();


    if (level_number == 0) {
        if (time <= 30) {
            title = new Sprite("Resources/Platforms/back1.png");
        }
        else if (time < 50) {
            title = new Sprite("Resources/Platforms/back2.png");
        }
        else if (time > 50) {
            title = new Sprite("Resources/Platforms/back3.png");
        }
    }
    else if (level_number == 1) {
        if (time <= 30) {
            title = new Sprite("Resources/Platforms/back1.png");
        }
        else if (time < 50) {
            title = new Sprite("Resources/Platforms/back2.png");
        }
        else if (time > 50) {
            title = new Sprite("Resources/Platforms/back3.png");
        }
    }
    else if (level_number == 2) {
        if (time <= 30) {
            title = new Sprite("Resources/Platforms/back1.png");
        }
        else if (time < 50) {
            title = new Sprite("Resources/Platforms/back2.png");
        }
        else if (time > 50) {
            title = new Sprite("Resources/Platforms/back3.png");
        }
    }

    // Adiciona botões
    callback_function play_sfx = []() { FireboyWatergirl::audio->Play(BUTTON_SELECT); };
    failed = static_cast<Level*>(FireboyWatergirl::last_level)->failed();
    if (level_number != 1 || failed) {
        play_button = new Button(window->CenterX() - 6, window->CenterY(), failed ? "Try Again" : "Continue", *font, Color{ 1, 1, 1, 1 }, Color{ 1, 1, 0, 1 }, 1.75, play_sfx);
        scene->Add(play_button, STATIC);
    }
    menu_button = new Button(window->CenterX() - 6, window->CenterY() + 78, "Main Menu", *font, Color{ 1, 1, 1, 1 }, Color{ 1, 1, 0, 1 }, 1.65, play_sfx);

    scene->Add(menu_button, STATIC);
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
    title->Draw(window->CenterX(), window->CenterY(), Layer::FRONT);
}

void GameOver::Finalize()
{
    delete scene;
    delete font;
    delete title;
}
