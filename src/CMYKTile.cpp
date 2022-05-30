//Copyright (c) 2021 Alex Gray

#include "CMYKTile.h"
#include "Utils.h"
#include <bitmap_image.hpp>
#include <math.h>
#include <iostream>
#include <sstream>

CMYKTile::CMYKTile(std::unique_ptr<TranslatedPixel> && pixel):
Tile(std::move(pixel))
{
}

OBJData CMYKTile::generateOBJData(int & faceStartingNumber, const float xStart,  const float xEnd,  const float yStart,  const float yEnd) const
{
    std::vector<vertices> verticesVec;
    std::vector<faces> facesVec;
    verticesVec.reserve(INNER_TILE_VERTS);
    facesVec.reserve(INNER_TILE_FACES);
    TileOptions tOptions = TileOptions::none;

    std::vector<vertices> colorVerts = getVertices(xStart, xEnd, yStart, yEnd);
    verticesVec.insert(std::end(verticesVec), std::begin(colorVerts), std::end(colorVerts));
        
    std::vector<faces> colorFaces = getFaces(faceStartingNumber, tOptions);
    facesVec.insert(std::end(facesVec), std::begin(colorFaces), std::end(colorFaces));

    return std::make_pair(verticesVec, facesVec);
}

OBJData CMYKTile::getOBJDataHorizontal(int & faceStartingNumber, const int color) const
{
    float xStart = 0.0f;
    float xEnd = 0.0f;
    float yStart = 0.0f; 
    float yEnd = 0.0f;

    return generateOBJData(faceStartingNumber, xStart, xEnd, yStart, yEnd);
}

OBJData CMYKTile::getOBJDataVertical(int & faceStartingNumber, const int color) const
{
    float xStart = 0.0f;
    float xEnd = 0.0f;
    float yStart = 0.0f; 
    float yEnd = 0.0f;
    float bufferVal = Utils::getBufferOBJValue(m_tileSizeMM);

    switch(color)
    {
        case 0:
            //bottom x, full y
            xStart = 0.0f;
            xEnd = m_tileSizeMM - m_translatedPixel->getTranslatedColorValue(color);
            yStart = 0.0f;
            yEnd = m_tileSizeMM + bufferVal; 
            break;
        case 1:
            //upper x, full y
            xStart = m_tileSizeMM - ((m_tileSizeMM) - (m_translatedPixel->getTranslatedColorValue(color))); 
            xEnd = m_tileSizeMM + bufferVal;
            yStart = 0.0f; 
            yEnd = m_tileSizeMM + bufferVal; 
            break;
        case 2:
            //full x, bottom y
            xStart = 0.0f;
            xEnd = m_tileSizeMM + bufferVal;
            yStart = 0.0f; 
            yEnd = m_tileSizeMM - m_translatedPixel->getTranslatedColorValue(color);
            break;
        case 3:
            //full x, upper y
            xStart = 0.0f;
            xEnd = m_tileSizeMM + bufferVal;
            yStart = m_tileSizeMM - ((m_tileSizeMM) - (m_translatedPixel->getTranslatedColorValue(color))); 
            yEnd = m_tileSizeMM + bufferVal; 
            break;
    }



    return generateOBJData(faceStartingNumber, xStart, xEnd, yStart, yEnd);
}

OBJData CMYKTile::getOBJData(int & faceStartingNumber, const int color) const
{
    OBJData verticals = getOBJDataVertical(faceStartingNumber, color);
    return verticals;

    /**
        + - - - - - +
        | X X X X X |
        | - - - X X |
        | - - - X X |
        | - - - X X |
        + - - - - - +
    **/

    // //4 colours 
    // float xStart = 0.0f;
    // float xEnd = m_tileSizeMM / 2;
    // float yStart = 0.0f; // always 0, only end will change (pixel strength)
    // float yEnd = 0.0f + bufferVal; // strength of color

    // return generateOBJData(faceStartingNumber, xStart, xEnd, yStart, yEnd);
    // return std::make_pair(verticesVec, facesVec);
}

std::string CMYKTile::toString() const
{
    std::ostringstream ss;
    ss << "CMYKTile - x:" << getX() << " y:" << getY() << "\n" << m_translatedPixel->toString();
    return ss.str();
}

std::string CMYKTile::type() const
{
    return "CMYKTile";
}
// int CMYKTile::numberOfColors() const
// {
//     return m_translatedPixel->getColorArraySize();
// }
