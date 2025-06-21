#include "Background.h"
#include <string>

Background::Background(std::string& background, Color tint) : color(tint)
{
    MoveTo(window->CenterX(), window->CenterY(), Layer::BACK);

    // cria sprite do plano de fundo 
    backg = new Sprite(background);
}

Background::~Background()
{
    delete backg;
}

void Background::Update()
{
    
}

void Background::Draw()
{
    // desenha plano de fundo
    backg->Draw(x, y, Layer::MIDDLE, 1.0f, 0.0f, color);
}
