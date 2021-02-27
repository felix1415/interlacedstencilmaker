//Copyright (c) 2021 Alex Gray

#pragma once
#include "TranslatedPixel.h"
#include <iostream>
#include <string>

enum TileType
{
    InnerTile,
    BorderTile,
};

/* Tile will generate the obj values for the space it is to occupy in 
the stencil based on the strength of the colour it is representing. 
It doesn't need to know which colour, only how strong that colour 
should appear*/
class Tile
{
    public:
        Tile(std::vector<Pixel> & pixels, const int steps);
        // void getOBJ();
        
    private:
        TranslatedPixel m_pixel;
        const int m_steps;

};