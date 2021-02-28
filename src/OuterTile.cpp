//Copyright (c) 2021 Alex Gray

#include "OuterTile.h"

OuterTile::OuterTile(const uint16_t x, const uint16_t y, const float m_tileSizeMM):
Tile(x, y, m_tileSizeMM)
{
    //we've not been given a pixel just a position, so we must be an 
    //outer tile
}