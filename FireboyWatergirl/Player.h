#pragma once
#include "Types.h"                      // tipos espec�ficos da engine
#include "Object.h"                     // interface de Object
#include "Animation.h"                  // anima��o de sprites
#include "Controller.h"
#include "Vector.h"

enum State { IDLE, RUNNING, JUMPING, FALLING };

class Player : public Object
{
private:
    Controller* gamepad;
    bool controllerOn, is_xbox_controller;

    Image* tiles;
    Timer* animation_timer;

    Animation    * anim_head_idle;      // anima��o do personagem
    Animation    * anim_head_fall;      // anima��o do personagem
    Animation    * anim_head_jump;      // anima��o do personagem
    Animation    * anim_head_run;       // anima��o do personagem
                                        
    Animation    * anim_body_run;       // anima��o do personagem
    Animation    * anim_body_idle;      // anima��o do personagem

    Animation    * anim_win;

    Animation    * current_anim_head;
    Animation    * current_anim_body;

    Vector       * velocity;
    int          level;                  
    float        initial_posX, initial_posY;
    bool         is_alive, is_fireboy, ready_next_level, enable_controls;

    State state;

    void updateState();
    void slowDown();

public:
    Player(bool is_fireboy, Controller* controller, bool controllerOn, bool xboxOn);
    ~Player();                          

    void Reset();                       // volta ao estado inicial
    void Reset(int level);              
    int LevelNumber();                  // �ltimo n�vel finalizado                     
    void LevelNumber(int level);                                          
    float Bottom();                     // coordenadas da base
    float Top();                        // coordenadas do topo
    float Height();                        
    float Width();                        
    bool  IsFireboy();
    bool  IsAlive();
    bool  IsStill();
    bool  IsReadyNextLevel();

    void setInitialPosition(float x, float y);
    void setScale(float scale);
    void enableControls();
    void disableControls();

    void OnCollision(Object * obj);     // resolu��o da colis�o
    void Update();                      // atualiza��o do objeto
    void Draw();                        // desenho do objeto
};

inline int Player::LevelNumber()
{ return level; }

inline void Player::LevelNumber(int level)
{ this->level = level; }

inline float Player::Bottom()
{ return y + Height()/2; }

inline float Player::Top()
{ return y - Height()/2; }

inline bool Player::IsFireboy()
{ return is_fireboy; }

inline bool Player::IsAlive() 
{ return is_alive; }

inline bool Player::IsStill()
{ return velocity->Magnitude() == 0; }

inline bool Player::IsReadyNextLevel() 
{ return ready_next_level; }

inline void Player::setInitialPosition(float x, float y)
{ initial_posX = x; initial_posY = y; }

inline void Player::setScale(float scale)
{ ScaleTo(scale); }

inline void Player::enableControls()
{ enable_controls = true; }

inline void Player::disableControls()
{ enable_controls = false; }
