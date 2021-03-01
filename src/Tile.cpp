//Copyright (c) 2021 Alex Gray

#include "Tile.h"
#include "Utils.h"
#include <bitmap_image.hpp>
#include <math.h>
#include <iostream>
#include <sstream>

Tile::Tile(TranslatedPixel && pixel):
Position(pixel.getX() + 1, pixel.getY() + 1),
m_translatedPixel(std::move(pixel)),
m_tileSizeMM(pixel.getTileSizeMM()),
m_type(TileType::innerTile)
{
    //our tile is going to fit into a large grid of tiles, if we give 
    //is a pixel, it's an inner tile, we plus one to the inner tiles 
    //so that border tiles can fill their space
}

Tile::Tile(const uint16_t x, const uint16_t y, const float tileSizeMM):
Position(x, y),
m_tileSizeMM(tileSizeMM),
m_type(TileType::borderTile)
{
}

std::pair<std::vector<vertices>,std::vector<faces>> Tile::getOBJData(const bitmap_image::color_plane color, int & faceStartingNumber) const
{
    std::vector<vertices> verticesVec;
    std::vector<faces> facesVec;
    verticesVec.reserve(INNER_TILE_VERTS); // 8 vertices * 3 colors
    facesVec.reserve(INNER_TILE_FACES);
    float xStart = 0.0f;
    float xEnd = m_tileSizeMM / 3;
    float yStart = 0.0f; // always 0, only end will change (pixel strength)
    float yEnd = 0.0f; // strength of color

    const size_t numberOfColors = m_translatedPixel.getColorArray().size();
    // for(const auto & color : m_translatedPixel.getColorArray())
    for(size_t i = 0; i < numberOfColors; i++)
    {
        //y height is always the length of the tile, unless the TP has a 
        //value to reduce it (allowing color through)
        if(((bitmap_image::color_plane)i == color))
        {
            yEnd = (m_tileSizeMM - m_translatedPixel.getTranslatedColorValue((bitmap_image::color_plane)i));
        }
        else
        {
            yEnd = m_tileSizeMM;
        }
        
        std::vector<vertices> colorVerts = getVertices(xStart, xEnd, yStart, yEnd);
        verticesVec.insert(std::end(verticesVec), std::begin(colorVerts), std::end(colorVerts));
        
        //next color prep
        xStart = xEnd;
        xEnd += m_tileSizeMM / 3;

        std::vector<faces> colorFaces = getFaces(faceStartingNumber);
        facesVec.insert(std::end(facesVec), std::begin(colorFaces), std::end(colorFaces));
    }

    return std::make_pair(verticesVec, facesVec);
}

std::vector<vertices> Tile::getVertices(float xStart, float xEnd, float yStart, float yEnd) const
{
    std::vector<vertices> verticesToReturn = {
        {(float)getX() * m_tileSizeMM + xStart,     (float)getY() + yStart,      0},
        {(float)getX() * m_tileSizeMM + xEnd,       (float)getY() + yStart,      0},
        {(float)getX() * m_tileSizeMM + xEnd,       (float)getY() + yEnd,        0},
        {(float)getX() * m_tileSizeMM + xStart,     (float)getY() + yEnd,        0},
        {(float)getX() * m_tileSizeMM + xStart,     (float)getY() + yStart,      0.6},
        {(float)getX() * m_tileSizeMM + xEnd,       (float)getY() + yStart,      0.6},
        {(float)getX() * m_tileSizeMM + xEnd,       (float)getY() + yEnd,        0.6},
        {(float)getX() * m_tileSizeMM + xStart,     (float)getY() + yEnd,        0.6},
    };

    return verticesToReturn;
}

std::vector<faces> Tile::getFaces(int & faceStartingNumber) const
{
    std::vector<faces> facesToReturn = {
        {uint16_t(1+faceStartingNumber), uint16_t(2+faceStartingNumber), uint16_t(3+faceStartingNumber), uint16_t(4+faceStartingNumber)},
        {uint16_t(5+faceStartingNumber), uint16_t(6+faceStartingNumber), uint16_t(7+faceStartingNumber), uint16_t(8+faceStartingNumber)},
        {uint16_t(1+faceStartingNumber), uint16_t(2+faceStartingNumber), uint16_t(6+faceStartingNumber), uint16_t(5+faceStartingNumber)},
        {uint16_t(2+faceStartingNumber), uint16_t(3+faceStartingNumber), uint16_t(7+faceStartingNumber), uint16_t(6+faceStartingNumber)},
        {uint16_t(3+faceStartingNumber), uint16_t(4+faceStartingNumber), uint16_t(8+faceStartingNumber), uint16_t(7+faceStartingNumber)},
        {uint16_t(4+faceStartingNumber), uint16_t(1+faceStartingNumber), uint16_t(5+faceStartingNumber), uint16_t(8+faceStartingNumber)},
    };

    faceStartingNumber += 8;

    return facesToReturn;
}
