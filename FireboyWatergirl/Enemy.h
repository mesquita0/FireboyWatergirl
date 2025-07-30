#pragma once
#include "Animation.h"
#include "Object.h"
#include "Vector.h"

class Enemy : public Object {
private:
    float speed;     // Velocidade de movimentação

    Animation* anim;
    Vector* velocity;

    bool follow_fireboy;

public:
    Enemy(float x, float y, float speed = 1.0f);
    ~Enemy();

    void Update();
    void Draw();

    float X() const { return x; }
    float Y() const { return y; }
};
