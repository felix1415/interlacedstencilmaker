//Copyright (c) 2021 Alex Gray

#pragma once
#include "TranslatedPixel.h"
#include "Position.h"
#include "OBJ.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>

enum TileType
{
    innerTile,
    borderTile,
};

enum TileOptions
{
    none = 0,
    omitSides,
    omitBottom,
    omitEnds,
};

const static int INNER_TILE_VERTS = 8 * 3;
const static int INNER_TILE_FACES = 6 * 3;
/* Tile will generate the obj values for the space it is to occupy in 
the stencil based on the strength of the colour it is representing. 
It doesn't need to know which colour, only how strong that colour 
should appear*/
class Tile : public Position
{

    public: //inner grouped color tile
        Tile(std::unique_ptr<TranslatedPixel> && pixel);

    protected: //outerTile
        Tile(const uint16_t x, const uint16_t y, const float m_tileSizeMM);

    public:
        //pixel functions we don't want, tiles are colour independent
        const rgb_t & getColor() const = delete;
        void calculateTranslatedColourValue() = delete;

        virtual OBJData getOBJData(int & faceStartingNumber, const bitmap_image::color_plane color) const;
        virtual std::string toString() const;

    protected:
        std::vector<vertices> getVertices(float xStart, float xEnd, float yStart, float yEnd) const;
        std::vector<faces> getFaces(int & faceStartingNumber, const TileOptions tileOptions = TileOptions::none) const;

    private:
        std::unique_ptr<TranslatedPixel>  m_translatedPixel;
    protected:
        const float m_tileSizeMM;
    private:
        const TileType m_type;

};