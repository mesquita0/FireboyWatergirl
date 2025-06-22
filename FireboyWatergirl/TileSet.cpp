/**********************************************************************************
// TileSet (Código Fonte)
// 
// Criação:     17 Abr 2012
// Atualização: 23 Ago 2023
// Compilador:  Visual C++ 2022
//
// Descrição:   Representa uma folha de sprites
//
**********************************************************************************/

#include "TileSet.h"
#include "Image.h"

// -------------------------------------------------------------------------------

TileSet::TileSet(string filename, uint tileWidth, uint tileHeight, uint numCols, uint numTiles, uint offset_x, uint offset_y) :
    image(new Image(filename)), 
    width(tileWidth), 
    height(tileHeight), 
    columns(numCols),    
    size(numTiles),
    offset_x(offset_x),
    offset_y(offset_y)
{

}

TileSet::TileSet(string filename, uint numLines, uint numCols, uint offset_x, uint offset_y) :
    image(new Image(filename)),
    columns(numCols),
    size(numLines* numCols),
    offset_x(offset_x),
    offset_y(offset_y)
{
    width = image->Width() / numCols;
    height = image->Height() / numLines;
}

TileSet::TileSet(Image* image, uint tileWidth, uint tileHeight, uint numLines, uint numCols, uint offset_x, uint offset_y) :
    image(image),
    width(tileWidth),
    height(tileHeight),
    columns(numCols),
    size(numLines* numCols),
    offset_x(offset_x),
    offset_y(offset_y)
{
    owns_image = false;
}

// -------------------------------------------------------------------------------

TileSet::~TileSet()
{
    // libera memória ocupada pela imagem
    if (image && owns_image)
    {
         delete image;
    }
}

// -------------------------------------------------------------------------------