//Copyright (c) 2021 Alex Gray

#pragma once
#include "Tile.h"

class OuterTile : public Tile
{
    public:
        OuterTile(const float x, const float y, const float endX, const float endY, const float tileSizeMM);

        OBJData getOBJData(int & faceStartingNumber, const int = 0) const override;

        std::string toString() const override;

    private:
        const float m_endX;
        const float m_endY;

};