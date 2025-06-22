/**********************************************************************************
// Animation (Código Fonte)
// 
// Criação:     28 Set 2011
// Atualização: 13 Set 2023
// Compilador:  Visual C++ 2022
//
// Descrição:   Classe para animar sequências em folha de sprites
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

    // sempre inicia a animação pelo primeiro quadro
    frame = iniFrame = 0;

    // o último quadro é sempre um a menos que o número de quadros
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
    
    // animação iniciada (começa a contar o tempo)
    timer.Start();                

    // nenhuma sequência selecionada
    sequence = nullptr;
}

// --------------------------------------------------------------------------------

Animation::~Animation()
{
    if (tiles)
        delete tiles;

    if (!table.empty())
    {
        // liberando memória dos vetores dinâmicos de sequências
        for (const auto & [id,seq] : table)
            delete seq.first;
    }
}

// --------------------------------------------------------------------------------

void Animation::Add(uint id, uint * seq, uint seqSize)
{
    // cria nova sequência de animação
    AnimSeq newSeq(new uint[seqSize], seqSize);

    // copia vetor com a sequência de quadros
    memcpy(newSeq.first, seq, sizeof(uint) * seqSize);

    // insere nova sequência
    table[id] = newSeq;

    // seleciona sequência recém inserida
    sequence = newSeq.first;

    // inicia animação pelo primeiro quadro da sequência
    iniFrame = frame = 0;

    // o último quadro é sempre um a menos que o número de quadros
    endFrame = seqSize - 1;
}

// --------------------------------------------------------------------------------

void Animation::Select(uint id)
{
    const auto & [seq, size] = table[id];

    // se uma nova sequência for selecionada
    if (sequence != seq)
    {
        // aponta para nova sequência
        sequence = seq;

        // reinicia a sequência
        iniFrame = 0;
        endFrame = size - 1;

        // se o frame atual está fora da sequência
        if (frame > endFrame)
            frame = 0;
    }
}


// ---------------------------------------------------------------------------------

void Animation::NextFrame()
{
    // passa para o próximo quadro após espaço de tempo estipulado em animDelay
    if (timer.Elapsed(animDelay))
    {
        frame++;

        // se chegou ao fim da animação
        if (frame > endFrame)
        {
            // se a animação estiver em loop
            if (animLoop)
            {
                // volta ao primeiro quadro 
                frame = 0;
                timer.Start();
            }
            else
            {
                // fica no último quadro
                frame = endFrame;
            }
        }
        else
        {
            // começa a contar o tempo do novo frame
            timer.Start();
        }
    }
}

// ---------------------------------------------------------------------------------

void Animation::Draw(uint aFrame, float x, float y, float z, float scale, float rotation, Color color)
{
    // configura dados básicos
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