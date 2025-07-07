#pragma once
#include "Object.h"
#include "Animation.h"
#include "Font.h"
#include "TileSet.h"
#include "Types.h"
#include <string>

typedef void (*callback_function)(void);

class Button : public Object
{
private:
    std::string text;
    Font font;
    Color color_selected, color_unselected;
    bool is_selected, was_selected;
    callback_function on_select;

public:
    Button(float posX, float posY, std::string text, Font& font, Color color_selected, Color color_unselected, float scale = 1, callback_function on_select = nullptr);

    void Select() { is_selected = true; }
    void UnSelect() { is_selected = false; }
  
    void Update();
    void Draw();          

    void OnCollision(Object* obj);
};
