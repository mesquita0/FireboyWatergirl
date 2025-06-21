#pragma once
#include "Types.h"                      // tipos espec�ficos da engine
#include "Object.h"                     // interface de Object
#include "Sprite.h"                     // background � composto por sprites
#include <string>

class Background : public Object
{
private:
    Sprite* backg;                      // sprite pano de fundo

    Color color;                        // cor do pano de fundo

public:
    Background(std::string& background, Color tint);     // construtor
    ~Background();                                       // destrutor
                                                         
    void Update();                                       // atualiza��o do objeto
    void Draw();                                         // desenho do objeto
};
