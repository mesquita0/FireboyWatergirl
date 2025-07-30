#include "Enemy.h"
#include "FireboyWatergirl.h" 
#include "WorldEntity.h" 

#include <cmath>

Enemy::Enemy(float x, float y, float speed)
    : speed(speed)
{
    anim = new Animation(TileSet("Resources/Ghost.png", 16, 27, 3, 3, 14, 0), 0.3f, true);
    velocity = new Vector();
    velocity->ScaleTo(speed);

    float width = anim->tileSet()->TileWidth();
    float height = anim->tileSet()->TileHeight();

    BBox(new Rect(
        -1.0f * (width) / 2.0f,
        -1.0f * (height) / 2.0f,
        (width) / 2.0f,
        (height) / 2.0f));

    follow_fireboy = rand() % 2 == 0;

    MoveTo(x, y);
    type = ENEMY;
}

void Enemy::Update() {
    anim->NextFrame();

    Player * player = follow_fireboy ? FireboyWatergirl::fireboy : FireboyWatergirl::watergirl;

    float targetX = player->X();
    float targetY = player->Y();

    float dx = targetX - x;
    float dy = targetY - y;

    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0.01f) {
        float angleDegrees = std::atan2(dy, dx) * 180.0f / 3.14159265f;

        velocity->Angle(angleDegrees);
        velocity->ScaleTo(speed);

        Translate(velocity->XComponent() * gameTime, velocity->YComponent() * gameTime);
    }
}

void Enemy::Draw() {
    bool mirror_x = (velocity->XComponent() < 0);

    anim->Draw(x, y, Layer::UPPER, 3, 0, mirror_x);
}

Enemy::~Enemy() {
    delete velocity;
    delete anim;
}
