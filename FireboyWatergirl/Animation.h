/**********************************************************************************
// Animation (Arquivo de Cabeçalho)
// 
// Criação:     28 Set 2011
// Atualização: 13 Set 2023
// Compilador:  Visual C++ 2022
//
// Descrição:   Classe para animar sequências em folha de sprites
//
**********************************************************************************/

#ifndef _PROGJOGOS_ANIMATION_H_
#define _PROGJOGOS_ANIMATION_H_

// ---------------------------------------------------------------------------------

#include "Sprite.h"              // tipo sprite para renderização
#include "TileSet.h"             // folha de sprites da animação
#include "Timer.h"               // tempo entre quadros da animação
#include "Types.h"               // tipos específicos da engine

#include <unordered_map>         // biblioteca STL                
using std::unordered_map;        // usando tabelas de dispersão
using std::pair;                 // usando pares de elementos

// seqüência <vetor, tam>
using AnimSeq = pair<uint*, uint>;

// tabela de dispersão <uint, AnimSeq>
using HashTable = unordered_map<uint, AnimSeq>;

// ---------------------------------------------------------------------------------

class Animation
{
private:
    static Timer shared_timer;

    uint  iniFrame;             // quadro inicial da seqüência
    uint  endFrame;             // quadro final da seqüência
    uint  frame;                // quadro atual da animação    
    bool  animLoop;             // animação em loop infinito
    float animDelay;            // espaço de tempo entre quadros (em segundos)
    Timer * timer;              // medidor de tempo entre quadros da animação
    TileSet * tiles;            // ponteiro para folha de sprites da animação
    SpriteData sprite;          // sprite a ser desenhado
    HashTable table;            // tabela com seqüências de animação
    uint * sequence;            // seqüência atualmente selecionada
    float offset_x, offset_y;
    bool used_shared_timer;

public:
    Animation(TileSet tiles, float delay, bool repeat, bool use_shared_timer = false);                
    ~Animation();                                                           

    void Add(uint id, uint * seq, uint seqSize);    // adiciona sequência de animação
    void Select(uint id);                           // seleciona sequência atual 

    void Draw(                                      // desenha o quadro atual da animação
        float x, float y, float z = Layer::MIDDLE,  // coordenadas da tela
        float scale = 1.0f, float rotation = 0.0f,  // escala e rotação
        bool mirror_x = false,
        Color color = { 1, 1, 1, 1 });              // efeito de cor

    void Draw(                                      // desenha um quadro da folha de sprites    
        uint aFrame,                                // quadro da folha a desenhar
        float x, float y, float z = Layer::MIDDLE,  // coordenadas da tela
        float scale = 1.0f, float rotation = 0.0f,  // escala e rotação
        bool mirror_x = false,
        Color color = { 1, 1, 1, 1 });              // efeito de cor

    void Frame(uint aFrame);                        // define o quadro atual da animação
    uint Frame();                                   // retorna o quadro de animação ativo
    void Delay(float delay);                        // define o tempo entre quadros
    TileSet* tileSet();
    bool timeElapsed();

    bool Inactive();                                // verifica se a animação já encerrou
    void NextFrame();                               // passa para o próximo frame da animação
    void Restart();                                 // reinicia a animacão (pelo primeiro frame da seqüência)
}; 

// ---------------------------------------------------------------------------------
// funções membro inline

// desenha quadro atual da animação
inline void Animation::Draw(float x, float y, float z, float scale, float rotation, bool mirror_x, Color color)
{ sequence ? Draw(sequence[frame], x, y, z, scale, rotation, mirror_x, color) : Draw(frame, x, y, z, scale, rotation, mirror_x, color); }

// define o frame atual da animação
inline void Animation::Frame(uint aFrame)
{ frame = aFrame; }

// retorna o frame de animação ativo
inline unsigned Animation::Frame()
{ return (sequence ? sequence[frame] : frame); }

inline void Animation::Delay(float delay)
{ animDelay = delay; }

inline TileSet* Animation::tileSet()
{ return tiles; }

inline bool Animation::timeElapsed()
{ return timer->Elapsed(animDelay); }

// verifica se a animação já encerrou
inline bool Animation::Inactive()
{ return !animLoop && (frame > endFrame || (frame == endFrame && timer->Elapsed(animDelay))); }

// reinicia a animacão (pelo primeiro frame da sequencia)
inline void Animation::Restart()
{ frame = 0; timer->Start(); }

// ---------------------------------------------------------------------------------

#endif