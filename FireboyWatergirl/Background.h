#pragma once
#include "Types.h"                      // tipos específicos da engine
#include "Object.h"                     // interface de Object
#include "Sprite.h"                     // background é composto por sprites
#include <string>

class Background : public Object
{
private:
    Sprite* backg;                      // sprite pano de fundo

    Color color;                        // cor do pano de fundo

public:
    Background(std::string& background, Color tint);     // construtor
    ~Background();                                       // destrutor
                                                         
    void Update();                                       // atualização do objeto
    void Draw();                                         // desenho do objeto
};
