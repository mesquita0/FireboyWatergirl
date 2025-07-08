#include "Player.h"
#include "Controller.h"
#include "FireboyWatergirl.h"
#include "WorldEntity.h"
#include "Level.h"

constexpr int controls[3][2] = { { 'W', VK_UP }, { 'A', VK_LEFT }, { 'D', VK_RIGHT } };
constexpr int key_up = 0;
constexpr int key_left = 1;
constexpr int key_right = 2;

constexpr float max_velocity_x = 200;

const Vector move_right = { 0, 200 };
const Vector jump = { 90, 370 };
const Vector move_left = { 180, 200 };
const Vector gravity = { 270,  700 };

const Vector slow_down_l = { 0,   20 };
const Vector slow_down_r = { 180, 20 };

Player::Player(bool is_fireboy) : is_fireboy(is_fireboy)
{
    if (is_fireboy) {
        tiles = new Image("Resources/Characters/Fireboy.png");

        anim_head_idle = new Animation(TileSet(tiles, 217, 338, 1, 5, 2, 162), 0.05f, true);
        anim_head_fall = new Animation(TileSet(tiles, 217, 338, 1, 5, 2, 532), 0.05f, true);
        anim_head_jump = new Animation(TileSet(tiles, 217, 338, 1, 5, 2, 900), 0.05f, true);

        anim_head_run = new Animation(TileSet(tiles, 344, 204, 1, 5, 2, 1267), 0.05f, true);

        anim_body_run = new Animation(TileSet(tiles, 116, 100, 1, 8, 106, 40), 0.05f, true);

        anim_body_idle = new Animation(TileSet(tiles, 102, 100, 1, 1, 3, 31), 0.05f, true);

        // TODO
        anim_win = new Animation(TileSet(tiles, 341, 204, 1, 5, 3, 1267), 0.05f, true);     
    }
    else {
        tiles = new Image("Resources/Characters/Watergirl.png");

        anim_head_idle = new Animation(TileSet(tiles, 208, 250, 1, 11, 2, 240), 0.03f, true);
        anim_head_fall = new Animation(TileSet(tiles, 205, 400, 1, 11, 2, 673), 0.05f, true);
        anim_head_jump = new Animation(TileSet(tiles, 209, 473, 1, 11, 2, 1256), 0.05f, true);

        anim_head_run = new Animation(TileSet(tiles, 378, 240, 1, 11, 2, 1877), 0.05f, true);

        anim_body_run = new Animation(TileSet(tiles, 118, 100, 1, 8, 95, 40), 0.05f, true);

        anim_body_idle = new Animation(TileSet(tiles, 102, 100, 1, 1, 3, 31), 0.05f, true);

        // TODO
        anim_win = new Animation(TileSet(tiles, 341, 204, 1, 5, 3, 1267), 0.05f, true);
    }

    // cria bounding box
    int offset_center = is_fireboy ? 100 * scale : 120 * scale;
    int offset_top = is_fireboy ? 0 : 35 * scale;
    BBox(new Rect(
        -1.0f * (Width()) / 2.0f,
        -1.0f * (Height() + offset_center) / 2.0f + offset_top,
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
    is_alive = true;

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
    if (velocity->YComponent() > 0 && velocity->Angle() > 85 && velocity->Angle() < 95) {
        state = JUMPING;
    }
    else if (abs(velocity->XComponent()) > 0.1) {
        state = RUNNING;
    }
    else if (velocity->YComponent() < -85) {
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
    case FINISH_PORTAL_ANY:
        ready_next_level = true;
        break;

    case FINISH_PORTAL_FIRE:
        if (is_fireboy) ready_next_level = true;
        break;

    case FINISH_PORTAL_WATER:
        if (!is_fireboy) ready_next_level = true;
        break;

    case THORN:
        is_alive = false;
        break;

    case MOVABLE_BOX:
        // Mantém plataforma para fora do personagem no eixo x (pode ser movida horizontalmente)
        // Mantém personagem para fora da plataforma no eixo y (não pode ser movida verticalmente)
        if (is_fireboy) {
            obj->Translate(-obj->BBox()->mtv_fire.XComponent(), 0);
            this->Translate(0, obj->BBox()->mtv_fire.YComponent());
            if (obj->BBox()->mtv_fire.Angle() == 270) velocity->YComponent(0);
        }      
        else {
            obj->Translate(-obj->BBox()->mtv_water.XComponent(), 0);
            this->Translate(0, obj->BBox()->mtv_water.YComponent());
            if (obj->BBox()->mtv_water.Angle() == 270) velocity->YComponent(0);
        }
        break;

    case MOVING_PLATFORM_X:
    case MOVING_PLATFORM_Y:
    case ROTATING_PLATFORM:
    case GROUND:
        if (
            !static_cast<Level*>(FireboyWatergirl::current_level)->scene->Collision(this, obj) ||
            (is_fireboy  && obj->BBox()->mtv_fire.Magnitude()  == 0) ||
            (!is_fireboy && obj->BBox()->mtv_water.Magnitude() == 0)
        ) {
            return;
        }

        // Mantém personagem fora da plataforma
        if (is_fireboy)
            Translate(obj->BBox()->mtv_fire.XComponent(), obj->BBox()->mtv_fire.YComponent());
        else
            Translate(obj->BBox()->mtv_water.XComponent(), obj->BBox()->mtv_water.YComponent());

        //
        float angle_fb = obj->BBox()->mtv_fire.Angle();
        float angle_wg = obj->BBox()->mtv_water.Angle();
        if (
            (is_fireboy && angle_fb > 225 && angle_fb < 315) ||
            (!is_fireboy && angle_wg > 225 && angle_wg < 315)
        ) {
            if (
                (is_fireboy && (angle_fb == 270)) ||
                (!is_fireboy && (angle_wg == 270)) ||
                abs(velocity->XComponent()) > max_velocity_x / 2.0f
            ) {
                velocity->YComponent(0);
            }

            // Ação pulo
            if (window->KeyPress(controls[key_up][is_fireboy]) || FireboyWatergirl::gamepad->XboxButton(ButtonA)) {
                velocity->Add(jump);
                FireboyWatergirl::audio->Play(is_fireboy ? FB_JUMP : WG_JUMP);
            }
        }

        else if (
            ((is_fireboy  && angle_fb == 90) ||
            (!is_fireboy && angle_wg == 90)) &&
            (velocity->YComponent() > 0)
        ) {
            velocity->YComponent(0);
        }

        else if (
            (is_fireboy  && (angle_fb == 0 || angle_fb == 180)) ||
            (!is_fireboy && (angle_wg == 0 || angle_wg == 180))
        ) {
            velocity->XComponent(velocity->XComponent() * 0.001);
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

    // Limitar velocidade
    if (abs(velocity->XComponent()) > max_velocity_x)
        velocity->XComponent((velocity->XComponent() > 0) ? max_velocity_x : -max_velocity_x);

    // Ação da gravidade sobre o personagem
    velocity->Add(gravity * gameTime);

    Translate(velocity->XComponent() * gameTime, -velocity->YComponent() * gameTime);

    current_anim_head->NextFrame();
    current_anim_body->NextFrame();
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
        if (velocity->YComponent() != 0) current_anim_body->Restart();

        rotation_head = (velocity->XComponent() > 0) ? velocity->Angle() : velocity->Angle() - 180;
        if (rotation_head > 45 && rotation_head < 90)        rotation_head = 45;
        else if (rotation_head < 315 && rotation_head > 270) rotation_head = 315;

        mirror_x = (velocity->XComponent() < 0);
        offset_y -= 20;
        if (is_fireboy) {
            offset_x = mirror_x ? -35 : 40;
        }
        else {
            offset_x = mirror_x ? -40 : 40;
            if (rotation_head != 0 && velocity->YComponent() > 0) offset_y -= 10;
            if (velocity->YComponent() < 0) offset_x -= 10;
        }
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
    
    float offset_body = is_fireboy ? 0 : -13;
    offset_y += offset_body;

    float head_x = x - offset_x * scale_head * scale;
    float head_y = y - offset_y * scale_head * scale;

    if (state != JUMPING) {
        float point_rotation_x = head_x - x;
        float point_rotation_y = 0;
        float angle_radians = rotation_head * (PI / 180);
        head_x += point_rotation_x * (1 - cos(angle_radians)) + point_rotation_y * sin(angle_radians);
        head_y += -point_rotation_x * sin(angle_radians) + point_rotation_y * (1 - cos(angle_radians));

        current_anim_body->Draw(x, y + (current_anim_body->tileSet()->TileHeight() / 2.0 - offset_body) * scale, z, scale, 0, mirror_x);
        current_anim_head->Draw(head_x, head_y, z, scale_head * scale, -rotation_head, mirror_x);
    }
    else {
        offset_y -= 25;
        current_anim_head->Draw(head_x, head_y, z, scale_head * scale, -rotation_head, mirror_x);
        current_anim_body->Draw(x, y + (current_anim_body->tileSet()->TileHeight() / 2.0 - offset_body) * scale, z, scale, 0, mirror_x);
    }
}
