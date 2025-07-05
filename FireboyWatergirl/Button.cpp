#include "Button.h"
#include <string>

Button::Button(float posX, float posY, std::string text, Font& font, Color color_selected, Color color_unselected, float scale) :
    text(text),
    font(font),
    color_selected(color_selected),
    color_unselected(color_unselected)
{
    is_selected = false;

    // cria bounding box
    int width = text.length() * font.getCharWidth();
    BBox(new Rect(
        -1.5 * font.getCharWidth(),
        - font.getCharHeight() / 2.0f,
        width - font.getCharWidth(),
        font.getCharHeight() / 2.0f));

    // ajusta posição e escala do botão
    MoveTo(posX, posY, Layer::FRONT);
    ScaleTo(scale);
}

void Button::Update()
{
    UnSelect();
}

void Button::Draw()
{
    font.Draw(x, y, text, (is_selected ? color_selected : color_unselected), z, scale);
}

void Button::OnCollision(Object* obj) {
    Select();
}
