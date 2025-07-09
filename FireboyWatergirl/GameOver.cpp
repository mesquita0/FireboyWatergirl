#include "Engine.h"
#include "GameOver.h"
#include "FireboyWatergirl.h"
#include "Home.h"
#include <format>

void GameOver::Init()
{
    title = new Sprite("Resources/Platforms/backback.png");
    font = new Font("Resources/Font.png");
    font->Spacing("Resources/FontMetrics.dat");


    int level = FireboyWatergirl::fireboy->LevelNumber();
    int time = static_cast<Level*>(FireboyWatergirl::levels[level])->timer.Elapsed();        


    if (level == 0) {
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
    else if (level == 1) { 
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
    else if (level == 2) {
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

}

void GameOver::Update()
{
    if (window->KeyPress(VK_ESCAPE) || window->KeyPress(VK_RETURN))
        FireboyWatergirl::NextLevel();
}

void GameOver::Draw()
{
    title->Draw(window->CenterX(), window->CenterY(), Layer::FRONT);
}

void GameOver::Finalize()
{
    delete font;
    delete title;
}
