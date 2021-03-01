//Copyright (c) 2021 Alex Gray

#pragma once
#include "Tile.h"

class OuterTile : public Tile
{
    public:
        OuterTile(const uint16_t x, const uint16_t y, const float tileSizeMM);

        OBJData getOBJData(int & faceStartingNumber) const;

        std::string toString() const override;

};