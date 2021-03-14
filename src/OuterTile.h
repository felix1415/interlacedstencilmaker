//Copyright (c) 2021 Alex Gray

#pragma once
#include "Tile.h"

class OuterTile : public Tile
{
    public:
        OuterTile(const uint16_t x, const uint16_t y, const uint16_t endX, const uint16_t endY, const float tileSizeMM);

        OBJData getOBJData(int & faceStartingNumber, const bitmap_image::color_plane = bitmap_image::color_plane::none) const override;

        std::string toString() const override;

    private:
        const uint16_t m_endX;
        const uint16_t m_endY;

};