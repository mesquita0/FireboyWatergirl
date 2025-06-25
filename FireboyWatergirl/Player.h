#pragma once
#include "Types.h"                      // tipos espec�ficos da engine
#include "Object.h"                     // interface de Object
#include "Animation.h"                  // anima��o de sprites
#include "Vector.h"

enum State { IDLE, RUNNING, JUMPING, FALLING };

class Player : public Object
{
private:
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
    int         level;                  // n�vel finalizado
    bool        is_alive, is_fireboy;

    State state;

    void updateState();

public:
    Player(bool is_fireboy);            // construtor
    ~Player();                          // destrutor

    void Reset();                       // volta ao estado inicial
    void Reset(int level);              
    int Level();                        // �ltimo n�vel finalizado                     
    void Level(int level);                                          
    float Bottom();                     // coordenadas da base
    float Top();                        // coordenadas do topo
    float Height();                        
    float Width();                        
    bool  IsAlive();

    void OnCollision(Object * obj);     // resolu��o da colis�o
    void Update();                      // atualiza��o do objeto
    void Draw();                        // desenho do objeto
};

inline int Player::Level()
{ return level; }

inline void Player::Level(int level)
{ this->level = level; }

inline float Player::Bottom()
{ return y + Height()/2; }

inline float Player::Top()
{ return y - Height()/2; }

inline bool Player::IsAlive() 
{ return is_alive; }
