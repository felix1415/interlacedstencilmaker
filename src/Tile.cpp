//Copyright (c) 2021 Alex Gray

#include "Tile.h"
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

std::pair<std::vector<vertices>,std::vector<faces>> Tile::getOBJData(ObjectNumberGenerator & objNumGen) const
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
        yEnd = m_translatedPixel.getTranslatedColorValue((bitmap_image::color_plane)i);
        std::vector<vertices> colorVerts = getVertices(xStart, xEnd, yStart, yEnd);
        verticesVec.insert(std::end(verticesVec), std::begin(colorVerts), std::end(colorVerts));
        xStart += xEnd;
        xEnd += xEnd;

        std::vector<faces> colorFaces = getFaces(objNumGen.nextValue());
        facesVec.insert(std::end(facesVec), std::begin(colorFaces), std::end(colorFaces));
    }

    return std::make_pair(verticesVec, facesVec);
}

std::vector<vertices> Tile::getVertices(float xStart, float xEnd, float yStart, float yEnd) const
{
    std::vector<vertices> verticesToReturn = {
        {(float)getX() + xEnd,     (float)getY() + yEnd,      0.6},
        {(float)getX() + xEnd,     (float)getY() + yEnd,      0},
        {(float)getX() + xStart,   (float)getY() + yStart,    0.6},
        {(float)getX() + xStart,   (float)getY() + yStart,    0},
        {(float)getX() + xStart,   (float)getY() + yEnd,      0},
        {(float)getX() + xStart,   (float)getY() + yEnd,      0.6},
        {(float)getX() + xEnd,     (float)getY() + yStart,    0},
        {(float)getX() + xEnd,     (float)getY() + yStart,    0.6},
    };

    return verticesToReturn;
}

std::vector<faces> Tile::getFaces(const int objNumber) const
{
    std::vector<faces> facesToReturn = {
        {uint16_t(1+objNumber), uint16_t(2+objNumber), uint16_t(3+objNumber)},
        {uint16_t(1+objNumber), uint16_t(3+objNumber), uint16_t(4+objNumber)},
        {uint16_t(5+objNumber), uint16_t(6+objNumber), uint16_t(7+objNumber)},
        {uint16_t(5+objNumber), uint16_t(7+objNumber), uint16_t(8+objNumber)},
        {uint16_t(5+objNumber), uint16_t(8+objNumber), uint16_t(1+objNumber)},
        {uint16_t(7+objNumber), uint16_t(6+objNumber), uint16_t(3+objNumber)},
        {uint16_t(8+objNumber), uint16_t(7+objNumber), uint16_t(1+objNumber)},
        {uint16_t(5+objNumber), uint16_t(3+objNumber), uint16_t(6+objNumber)},
        {uint16_t(4+objNumber), uint16_t(5+objNumber), uint16_t(1+objNumber)},
        {uint16_t(4+objNumber), uint16_t(3+objNumber), uint16_t(5+objNumber)},
        {uint16_t(2+objNumber), uint16_t(7+objNumber), uint16_t(3+objNumber)},
        {uint16_t(1+objNumber), uint16_t(7+objNumber), uint16_t(1+objNumber)},
    };

    return facesToReturn;
}

    // # Object Export From Tinkercad Server 2015

    // mtllib obj.mtl

    // o obj_0
    // v -80       90      1
    // v -80       90      0
    // v -80       100         0
    // v -80       100         1
    // v -100      100         1
    // v -100      100         0
    // v -100      90      0
    // v -100      90      1
    // # 8 vertices

    // g group_0_15277357

    // usemtl color_15277357
    // s 0

    // f 1     2   3
    // f 1     3   4
    // f 5     6   7
    // f 5     7   8
    // f 5     8   1
    // f 7     6   3
    // f 8     7   1
    // f 5     3   6
    // f 4     5   1
    // f 4     3   5
    // f 2     7   3
    // f 1     7   2
    // # 12 faces

    //  #end of obj_0


