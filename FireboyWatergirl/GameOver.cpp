#include "Engine.h"
#include "GameOver.h"
#include "FireboyWatergirl.h"
#include "Home.h"
#include <format>

void GameOver::Init()
{
    title = new Sprite("");
    font = new Font("Resources/Font.png");
    font->Spacing("Resources/FontMetrics.dat");
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
