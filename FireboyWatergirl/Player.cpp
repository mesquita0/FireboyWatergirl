#include "Player.h"
#include "Controller.h"
#include "FireboyWatergirl.h"

constexpr float player_scale = 0.2;

constexpr int controls[3][2] = { { 'w', VK_UP }, { 'a', VK_LEFT }, { 'd', VK_RIGHT } };
constexpr int key_up = 0;
constexpr int key_left = 1;
constexpr int key_right = 2;

const Vector move_right = { 0,   20 };
const Vector gravity = { 90,  500 };
const Vector move_left = { 180, 20 };
const Vector jump = { 270, 300 };

const Vector slow_down_l = { 0,   20 };
const Vector slow_down_r = { 180, 20 };

Player::Player(bool is_fireboy) : is_fireboy(is_fireboy)
{
    is_alive = true;

    if (is_fireboy) {
        tiles = new Image("Resources/Characters/Fireboy.png");

        anim_head_idle = new Animation(TileSet(tiles, 217, 338, 1, 4, 2, 162), 0.05f, true, true);
        anim_head_fall = new Animation(TileSet(tiles, 217, 338, 1, 4, 2, 532), 0.05f, true, true);
        anim_head_jump = new Animation(TileSet(tiles, 217, 338, 1, 4, 2, 900), 0.05f, true, true);

        anim_head_run = new Animation(TileSet(tiles, 344, 204, 1, 5, 2, 1267), 0.05f, true, true);

        anim_body_run = new Animation(TileSet(tiles, 72, 84, 1, 8, 108, 31), 0.05f, true, true);

        anim_body_idle = new Animation(TileSet(tiles, 102, 100, 1, 1, 3, 31), 0.05f, true, true);

        // TODO
        anim_win = new Animation(TileSet(tiles, 341, 204, 1, 5, 3, 1267), 0.05f, true, true);

        state = IDLE;
        current_anim_head = anim_head_idle;
        current_anim_body = anim_body_idle;

        // cria bounding box
        constexpr int offset_center = 100 * player_scale;
        BBox(new Rect(
            -1.0f * (Width()) / 2.0f,
            -1.0f * (Height() + offset_center) / 2.0f,
            (Width()) / 2.0f,
            (Height() - offset_center) / 2.0f));
    }
    else {
        // TODO
    }

    velocity = new Vector();

    // cria bounding box

    // posição inicial
    MoveTo(window->CenterX(), 24.0f, Layer::FRONT);
}

Player::~Player()
{
    delete velocity;
    delete anim_win;
    delete anim_body_idle;
    delete anim_body_run;
    delete anim_head_run;
    delete anim_head_jump;
    delete anim_head_fall;
    delete anim_head_idle;
    delete tiles;
}

void Player::Reset()
{
    // volta ao estado inicial

}

void Player::Reset(int level)
{
    Reset();
    this->level = level;
}

void Player::updateState()
{
    OutputDebugString(std::to_string(velocity->Angle()).c_str());
    OutputDebugString("\n");
    if (velocity->YComponent() < 0 && velocity->Angle() < 315 && velocity->Angle() > 225) {
        state = JUMPING;
    }
    else if (abs(velocity->XComponent()) > 0) {
        state = RUNNING;
    }
    else if (velocity->YComponent() > 0) {
        state = FALLING;
    }
    else {
        state = IDLE;
    }
}

float Player::Width() {
    return player_scale * anim_body_idle->tileSet()->TileWidth();
}

inline float Player::Height()
{
    return player_scale * (anim_body_idle->tileSet()->TileHeight() + anim_head_run->tileSet()->TileHeight());
}

void Player::slowDown() {
    if (velocity->XComponent() > -slow_down_r.XComponent())
        velocity->Add(slow_down_r);
    else if (velocity->XComponent() < -slow_down_l.XComponent())
        velocity->Add(slow_down_l);
    else
        velocity->XComponent(0);
}

void Player::OnCollision(Object* obj)
{
    switch (obj->Type())
    {

    }
}

void Player::Update()
{
    FireboyWatergirl::gamepad->XboxUpdateState();

    if (FireboyWatergirl::xboxOn) {
        //
        if (velocity->XComponent() * FireboyWatergirl::gamepad->XboxAnalog(ThumbLX) < 0)
            velocity->XComponent(0);

        velocity->Add(move_right * (static_cast<float>(FireboyWatergirl::gamepad->XboxAnalog(ThumbLX)) / SHRT_MAX));

        if (FireboyWatergirl::gamepad->XboxAnalog(ThumbLX) == 0)
            slowDown();
    }
    else {
        // Teclas direita e esquerda, ação de pulo é tratada no onCollision
        if (window->KeyDown(controls[key_left][is_fireboy])) {
            if (velocity->XComponent() > 0) velocity->XComponent(0);
            velocity->Add(move_left);
        }
        if (window->KeyDown(controls[key_right][is_fireboy])) {
            if (velocity->XComponent() < 0) velocity->XComponent(0);
            velocity->Add(move_right);
        }

        // Diminuir velocidade até parar se nenhuma ou as duas teclas de mover para os lados estão apertadas
        if (
            !window->KeyDown(controls[key_right][is_fireboy]) && !window->KeyDown(controls[key_left][is_fireboy]) ||
            window->KeyDown(controls[key_right][is_fireboy]) && window->KeyDown(controls[key_left][is_fireboy])
            ) {
            slowDown();
        }
    }

    // Ação da gravidade sobre o personagem
    velocity->Add(gravity * gameTime);

    // Limitar velocidade
    if (abs(velocity->XComponent()) > 200)
        velocity->XComponent((velocity->XComponent() > 0) ? 200 : -200);

    Translate(velocity->XComponent() * gameTime, velocity->YComponent() * gameTime);

    current_anim_head->NextFrame();
    current_anim_body->NextFrame();

    // JUMP : MOVE TO ONCOLLISION
    if (((Rect*)BBox())->Bottom() > window->Height()) {
        Translate(0, window->Height() - ((Rect*)BBox())->Bottom());
        velocity->YComponent(0);

        // Ação pulo
        if (window->KeyDown(controls[key_up][is_fireboy]) || FireboyWatergirl::gamepad->XboxButton(ButtonA)) {
            velocity->Add(jump);
        }
    }
}

inline void Player::Draw()
{
    updateState();
    bool mirror_x = false;
    float offset_y = current_anim_head->tileSet()->TileHeight() / 2.0 - 25;
    float rotation_head = 0;

    switch (state)
    {
    case IDLE:
        current_anim_body = anim_body_idle;
        current_anim_head = anim_head_idle;
        break;
    case RUNNING:
        current_anim_body = anim_body_run;
        current_anim_head = anim_head_run;
        mirror_x = (velocity->XComponent() < 0);
        rotation_head = max(min(velocity->Angle(), 45), -45);
        break;
    case JUMPING:
        current_anim_body = anim_body_idle;
        current_anim_head = anim_head_jump;
        offset_y = current_anim_head->tileSet()->TileHeight() / 2.0 - 145;
        break;
    case FALLING:
        current_anim_body = anim_body_idle;
        current_anim_head = anim_head_fall;
        break;
    default:
        break;
    }

    current_anim_body->Draw(x, y + (current_anim_body->tileSet()->TileHeight() / 2.0) * player_scale, z, player_scale, 0, mirror_x);
    current_anim_head->Draw(x - 5 * player_scale, y - offset_y * player_scale, z, player_scale, rotation_head, mirror_x);
}
