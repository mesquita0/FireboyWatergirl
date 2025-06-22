/**********************************************************************************
// Animation (C�digo Fonte)
// 
// Cria��o:     28 Set 2011
// Atualiza��o: 13 Set 2023
// Compilador:  Visual C++ 2022
//
// Descri��o:   Classe para animar sequ�ncias em folha de sprites
//
**********************************************************************************/

#include "Engine.h"
#include "Animation.h"
#include "Renderer.h"
#include "TileSet.h"

// ---------------------------------------------------------------------------------

Animation::Animation(TileSet tiles, float delay, bool repeat) : 
    animDelay(delay), 
    animLoop(repeat)
{
    // copia tileset
    this->tiles = new TileSet(tiles);

    // sempre inicia a anima��o pelo primeiro quadro
    frame = iniFrame = 0;

    // o �ltimo quadro � sempre um a menos que o n�mero de quadros
    endFrame = this->tiles->Size() - 1;

    // configura sprite
    sprite.width     = this->tiles->TileWidth();
    sprite.height    = this->tiles->TileHeight();
    sprite.texSize.x = float(this->tiles->TileWidth())  / this->tiles->Width();
    sprite.texSize.y = float(this->tiles->TileHeight()) / this->tiles->Height();
    sprite.texture   = this->tiles->View(); 
    
    // configura offsets
    offset_x = this->tiles->OffsetX() / static_cast<float>(this->tiles->Width());
    offset_y = this->tiles->OffsetY() / static_cast<float>(this->tiles->Height());
    
    // anima��o iniciada (come�a a contar o tempo)
    timer.Start();                

    // nenhuma sequ�ncia selecionada
    sequence = nullptr;
}

// --------------------------------------------------------------------------------

Animation::~Animation()
{
    if (tiles)
        delete tiles;

    if (!table.empty())
    {
        // liberando mem�ria dos vetores din�micos de sequ�ncias
        for (const auto & [id,seq] : table)
            delete seq.first;
    }
}

// --------------------------------------------------------------------------------

void Animation::Add(uint id, uint * seq, uint seqSize)
{
    // cria nova sequ�ncia de anima��o
    AnimSeq newSeq(new uint[seqSize], seqSize);

    // copia vetor com a sequ�ncia de quadros
    memcpy(newSeq.first, seq, sizeof(uint) * seqSize);

    // insere nova sequ�ncia
    table[id] = newSeq;

    // seleciona sequ�ncia rec�m inserida
    sequence = newSeq.first;

    // inicia anima��o pelo primeiro quadro da sequ�ncia
    iniFrame = frame = 0;

    // o �ltimo quadro � sempre um a menos que o n�mero de quadros
    endFrame = seqSize - 1;
}

// --------------------------------------------------------------------------------

void Animation::Select(uint id)
{
    const auto & [seq, size] = table[id];

    // se uma nova sequ�ncia for selecionada
    if (sequence != seq)
    {
        // aponta para nova sequ�ncia
        sequence = seq;

        // reinicia a sequ�ncia
        iniFrame = 0;
        endFrame = size - 1;

        // se o frame atual est� fora da sequ�ncia
        if (frame > endFrame)
            frame = 0;
    }
}


// ---------------------------------------------------------------------------------

void Animation::NextFrame()
{
    // passa para o pr�ximo quadro ap�s espa�o de tempo estipulado em animDelay
    if (timer.Elapsed(animDelay))
    {
        frame++;

        // se chegou ao fim da anima��o
        if (frame > endFrame)
        {
            // se a anima��o estiver em loop
            if (animLoop)
            {
                // volta ao primeiro quadro 
                frame = 0;
                timer.Start();
            }
            else
            {
                // fica no �ltimo quadro
                frame = endFrame;
            }
        }
        else
        {
            // come�a a contar o tempo do novo frame
            timer.Start();
        }
    }
}

// ---------------------------------------------------------------------------------

void Animation::Draw(uint aFrame, float x, float y, float z, float scale, float rotation, Color color)
{
    // configura dados b�sicos
    sprite.x = x;
    sprite.y = y;
    sprite.depth = z;
    sprite.scale = scale;
    sprite.rotation = rotation * XM_PI / 180.0f;
    sprite.color = color;

    // configura coordenadas da textura do sprite
    sprite.texCoord.x = offset_x + (aFrame % tiles->Columns()) * sprite.texSize.x;
    sprite.texCoord.y = offset_y + (aFrame / tiles->Columns()) * sprite.texSize.y;

    // adiciona o sprite na lista de desenho
    Engine::renderer->Draw(sprite);
}

// --------------------------------------------------------------------------------