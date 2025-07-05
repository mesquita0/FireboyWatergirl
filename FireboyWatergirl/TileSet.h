/**********************************************************************************
// TileSet (Arquivo de Cabe�alho)
// 
// Cria��o:     17 Abr 2012
// Atualiza��o: 23 Ago 2023
// Compilador:  Visual C++ 2022
//
// Descri��o:   Representa uma folha de sprites
//
**********************************************************************************/

#ifndef _PROGJOGOS_TILESET_H_
#define _PROGJOGOS_TILESET_H_

// ---------------------------------------------------------------------------------
// inclus�es

#include "Types.h"
#include "Image.h"
#include <string>
using std::string;

// ---------------------------------------------------------------------------------

class TileSet 
{
private:
    Image * image;                          // imagem da folha de sprites
    uint width;                             // largura de um quadro
    uint height;                            // altura de um quadro
    uint columns;                           // n�mero de colunas na folha
    uint size;                              // quantidade de quadros na folha
    uint offset_x, offset_y;
    bool owns_image = true;

public:
    TileSet() = default;

    TileSet(string filename,                // nome do arquivo
            uint tileWidth,                 // largura de um quadro
            uint tileHeight,                // altura de um quadro
            uint numCols,                   // n�mero de colunas na folha
            uint numTiles,                  // quantidade de quadros
            uint offset_x=0, 
            uint offset_y=0);         

    TileSet(string filename,                // nome do arquivo
            uint numLines,                  // n�mero de linhas na folha
            uint numCols,                   // n�mero de colunas na folha
            uint offset_x=0,
            uint offset_y=0);

    TileSet(Image* image,                
            uint tileWidth,                 
            uint tileHeight,                
            uint numCols,                   
            uint numTiles,                  
            uint offset_x = 0,
            uint offset_y = 0);

    TileSet(const TileSet& tileset); 

    ~TileSet();                                

    uint    TileWidth();                    // retorna a largura de um quadro
    uint    TileHeight();                   // retorna a altura de um quadro
    uint    Size();                         // retorna n�mero de quadros na folha
    uint    Columns();                      // retorna o n�mero de colunas na folha
    uint    Width();                        // retorna a largura da folha
    uint    Height();                       // retorna a altura da folha
    uint    OffsetX();
    uint    OffsetY();

    // retorna view da textura da folha
    ID3D11ShaderResourceView * View();        
};

// ---------------------------------------------------------------------------------
// fun��es membro inline

// retorna a largura de um quadro
inline uint TileSet::TileWidth()
{ return width; }

// retorna a altura de um quadro
inline uint TileSet::TileHeight()
{ return height; }

// retorna n�mero de quadros na folha
inline uint TileSet::Size()
{ return size; }

// retorna o n�mero de colunas na folha
inline uint TileSet::Columns()
{ return columns; }

// retorna a largura da folha
inline uint TileSet::Width()
{ return image->Width(); }

// retorna a altura da folha
inline uint TileSet::Height()
{ return image->Height(); }

// retorna resource view do tileset
inline ID3D11ShaderResourceView * TileSet::View()
{ return image->View(); }

inline uint TileSet::OffsetX()
{ return offset_x; }

inline uint TileSet::OffsetY()
{ return offset_y; }


// ---------------------------------------------------------------------------------

#endif




