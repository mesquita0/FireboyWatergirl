#include "Button.h"
#include <string>

Button::Button(float posX, float posY, std::string text, Font& font, Color color_selected, Color color_unselected, float scale, callback_function on_select) :
    text(text),
    font(font),
    color_selected(color_selected),
    color_unselected(color_unselected),
    on_select(on_select)
{
    is_selected = false;
    was_selected = false;

    // cria bounding box
    int width = text.length() * font.getCharWidth();
    BBox(new Rect(
        -(1 / scale) * font.getCharWidth(),
        - font.getCharHeight() / 2.0f,
        width - font.getCharWidth() * 3.1,
        font.getCharHeight() / 2.0f));

    // ajusta posição e escala do botão
    MoveTo(posX, posY, Layer::FRONT);
    ScaleTo(scale);
}

void Button::Update()
{
    was_selected = is_selected;
    UnSelect();
}

void Button::Draw()
{
    font.Draw(x, y, text, (is_selected ? color_selected : color_unselected), z, scale);
}

void Button::OnCollision(Object* obj) {
    if (!was_selected && on_select) {
        on_select();
        was_selected = true;
    }
        
    Select();
}
