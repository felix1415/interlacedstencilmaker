//Copyright (c) 2021 Alex Gray

#pragma once
#include "ObjectNumberGenerator.h"
#include "TranslatedPixel.h"
#include "Position.h"
#include "OBJ.h"
#include <iostream>
#include <string>

enum TileType
{
    colorInner,
    innerTile,
    borderTile,
};

const static int INNER_TILE_VERTS = 8 * 3;
const static int INNER_TILE_FACES = 12 * 3;

typedef std::pair<std::vector<vertices>,std::vector<faces>> OBJData;

/* Tile will generate the obj values for the space it is to occupy in 
the stencil based on the strength of the colour it is representing. 
It doesn't need to know which colour, only how strong that colour 
should appear*/
class Tile : public Position
{

    public: //inner grouped color tile
        Tile(TranslatedPixel && pixel);

    protected: //outerTile
        Tile(const uint16_t x, const uint16_t y, const float m_tileSizeMM);

    public:
        //pixel functions we don't want, tiles are colour independent
        const rgb_t & getColor() const = delete;
        void calculateTranslatedColourValue() = delete;

        virtual OBJData getOBJData(ObjectNumberGenerator & objNumGen) const;

    private:
        std::vector<vertices> getVertices(float xStart, float xEnd, float yStart, float yEnd) const;
        std::vector<faces> getFaces(const int objNumber) const;

        // OBJTile

        
    private:
        TranslatedPixel m_translatedPixel;
        float m_tileSizeMM;
        TileType m_type;

};