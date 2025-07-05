#pragma once
#include "Object.h"
#include "Animation.h"
#include "Font.h"
#include "TileSet.h"
#include "Types.h"
#include <string>

class Button : public Object
{
private:
    std::string text;
    Font font;
    Color color_selected, color_unselected;
    bool is_selected;

public:
    Button(float posX, float posY, std::string text, Font& font, Color color_selected, Color color_unselected, float scale = 1);

    void Select()   { is_selected = true; }
    void UnSelect() { is_selected = false; }
  
    void Update();
    void Draw();          

    void OnCollision(Object* obj);
};
