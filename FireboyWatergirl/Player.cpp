#include "Player.h"
#include "Controller.h"
#include "FireboyWatergirl.h"
#include "WorldEntity.h"

constexpr int controls[3][2] = { { 'W', VK_UP }, { 'A', VK_LEFT }, { 'D', VK_RIGHT } };
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

        anim_head_idle = new Animation(TileSet(tiles, 217, 338, 1, 5, 2, 162), 0.05f, true, true);
        anim_head_fall = new Animation(TileSet(tiles, 217, 338, 1, 5, 2, 532), 0.05f, true, true);
        anim_head_jump = new Animation(TileSet(tiles, 217, 338, 1, 5, 2, 900), 0.05f, true, true);

        anim_head_run = new Animation(TileSet(tiles, 344, 204, 1, 5, 2, 1267), 0.05f, true, true);

        anim_body_run = new Animation(TileSet(tiles, 72, 84, 1, 8, 108, 31), 0.05f, true, true);

        anim_body_idle = new Animation(TileSet(tiles, 102, 100, 1, 1, 3, 31), 0.05f, true, true);

        // TODO
        anim_win = new Animation(TileSet(tiles, 341, 204, 1, 5, 3, 1267), 0.05f, true, true);     
    }
    else {
        tiles = new Image("Resources/Characters/Watergirl.png");

        anim_head_idle = new Animation(TileSet(tiles, 208, 250, 1, 11, 2, 240), 0.03f, true);
        anim_head_fall = new Animation(TileSet(tiles, 205, 400, 1, 11, 2, 673), 0.05f, true);
        anim_head_jump = new Animation(TileSet(tiles, 209, 473, 1, 11, 2, 1256), 0.05f, true);

        anim_head_run = new Animation(TileSet(tiles, 378, 240, 1, 11, 2, 1877), 0.05f, true);

        anim_body_run = new Animation(TileSet(tiles, 72, 84, 1, 8, 108, 31), 0.05f, true);

        anim_body_idle = new Animation(TileSet(tiles, 102, 100, 1, 1, 3, 31), 0.05f, true);

        // TODO
        anim_win = new Animation(TileSet(tiles, 341, 204, 1, 5, 3, 1267), 0.05f, true);
    }

    // cria bounding box
    int offset_center = 100 * scale;
    BBox(new Rect(
        -1.0f * (Width()) / 2.0f,
        -1.0f * (Height() + offset_center) / 2.0f,
        (Width()) / 2.0f,
        (Height() - offset_center) / 2.0f));

    Reset();
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
    // estado inicial
    enable_controls = true;

    if (velocity)
        delete velocity;
    velocity = new Vector();

    state = IDLE;
    current_anim_head = anim_head_idle;
    current_anim_body = anim_body_idle;
    enable_controls = true;

    // posição inicial
    MoveTo(window->CenterX(), 24.0f, Layer::FRONT);
    ScaleTo(0.2); // default scale

    type = -1;
}

void Player::Reset(int level)
{
    Reset();
    this->level = level;
}

void Player::updateState()
{
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
    return scale * anim_body_idle->tileSet()->TileWidth();
}

inline float Player::Height()
{
    return scale * (anim_body_idle->tileSet()->TileHeight() + anim_head_run->tileSet()->TileHeight());
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
    case FINISH_PORTAL_FIRE:
        if (is_fireboy) ready_next_level = true;
        break;

    case FINISH_PORTAL_WATER:
        if (!is_fireboy) ready_next_level = true;
        break;

    case GROUND:
        // Mantém personagem fora da plataforma
        if (is_fireboy)
            Translate(obj->BBox()->mtv_fire.XComponent(), obj->BBox()->mtv_fire.YComponent());
        else
            Translate(obj->BBox()->mtv_water.XComponent(), obj->BBox()->mtv_water.YComponent());

        //
        float angle_fb = obj->BBox()->mtv_fire.Angle();
        float angle_wg = obj->BBox()->mtv_water.Angle();
        if (
            (is_fireboy  && angle_fb == 270) ||
            (!is_fireboy && angle_wg == 270)
        ) {
            velocity->YComponent(0);

            // Ação pulo
            if (window->KeyDown(controls[key_up][is_fireboy]) || FireboyWatergirl::gamepad->XboxButton(ButtonA)) {
                velocity->Add(jump);
            }
        }

        if (
            (is_fireboy  && angle_fb == 90) ||
            (!is_fireboy && angle_wg == 90)
        ) {
            velocity->YComponent(0);
        }

        if (
            (is_fireboy  && (angle_fb == 0 || angle_fb == 180)) ||
            (!is_fireboy && (angle_wg == 0 || angle_wg == 180))
        ) {
            velocity->XComponent(velocity->XComponent() * 0.0001);
        }

        break;
    }
}

void Player::Update()
{
    current_anim_head->NextFrame();
    current_anim_body->NextFrame();

    if (!enable_controls) return;

    // Resetar o estado em todo frame para conferir na colisão com o portal para o próximo nível
    ready_next_level = false;

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

    // Temporary
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
    float offset_x = is_fireboy ? 5 : -5;
    float offset_y = current_anim_head->tileSet()->TileHeight() / 2.0 - 25;
    float scale_head = is_fireboy ? 1 : 1.09;
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

    if (state != JUMPING) {
        current_anim_body->Draw(x, y + (current_anim_body->tileSet()->TileHeight() / 2.0) * scale, z, scale, 0, mirror_x);
        current_anim_head->Draw(x - offset_x * scale, y - offset_y * scale_head * scale, z, scale_head * scale, rotation_head, mirror_x);
    }
    else {
        offset_y -= 25;
        current_anim_head->Draw(x - offset_x * scale, y - offset_y * scale_head * scale, z, scale_head * scale, rotation_head, mirror_x);
        current_anim_body->Draw(x, y + (current_anim_body->tileSet()->TileHeight() / 2.0) * scale, z, scale, 0, mirror_x);
    }
}
