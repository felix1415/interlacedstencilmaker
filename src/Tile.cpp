//Copyright (c) 2021 Alex Gray

#include "Tile.h"
#include "Utils.h"
#include <bitmap_image.hpp>
#include <math.h>
#include <iostream>
#include <sstream>

Tile::Tile(std::unique_ptr<TranslatedPixel> && pixel):
Position(pixel->getX() + 1, pixel->getY() + 1),
m_translatedPixel(std::move(pixel)),
m_tileSizeMM(m_translatedPixel->getTileSizeMM()),
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

    const size_t numberOfColors = m_translatedPixel->getColorArray().size();
    // for(const auto & color : m_translatedPixel.getColorArray())
    for(size_t i = 0; i < numberOfColors; i++)
    {
        TileOptions tOptions = TileOptions::none;
        bool buffer = false;
        //y height is always the length of the tile, unless the TP has a 
        //value to reduce it (allowing color through)
        if(((bitmap_image::color_plane)i == color))
        {
            yEnd = (m_tileSizeMM - m_translatedPixel->getTranslatedColorValue((bitmap_image::color_plane)i));
            buffer = true;
            //if our translated color is at tileSize intensity, we can skip over it, the tile is too small to print
            //better than a equality operation, we're asking if it's less than the smallest possible value
            if(yEnd < (m_tileSizeMM / (m_translatedPixel->getSteps() + 1)) )
            {
                xStart = xEnd;
                xEnd += m_tileSizeMM / 3;
                tOptions = TileOptions::omitBottom;
                continue;
            }
        }
        else
        {
            yEnd = m_tileSizeMM;
            //OPTIMISATION, if we're one of the colors that aren't on this stencil, we can just 
            //extend our width out to cover the other color that isn't on this stencil and skip
            //the addition of the other vertices, this saves us a bunch of vertices (50%)
            if(color == i - 1  or (color == 2 and i == 0)) 
            {   
                xEnd += m_tileSizeMM / 3;
                tOptions = TileOptions::omitEnds;
            }
            else
            {
                //we're the colour being optimised for, continue
                continue;
            }            
        }
        
        std::vector<vertices> colorVerts = getVertices(xStart, xEnd, yStart, yEnd, buffer);
        verticesVec.insert(std::end(verticesVec), std::begin(colorVerts), std::end(colorVerts));
        
        std::vector<faces> colorFaces = getFaces(faceStartingNumber, tOptions);
        facesVec.insert(std::end(facesVec), std::begin(colorFaces), std::end(colorFaces));

        //next color prep
        xStart = xEnd;
        xEnd += m_tileSizeMM / 3;
    }

    return std::make_pair(verticesVec, facesVec);
}

std::vector<vertices> Tile::getVertices(float xStart, float xEnd, float yStart, float yEnd, const bool buffer) const
{
    float bufferVal = m_tileSizeMM * 0.0f;

    if(buffer)
    {
        //10% increase either side
        if(not ((yEnd - yStart) == m_tileSizeMM))
        {
            xStart = xStart + bufferVal;
            xEnd = xEnd - bufferVal;
        }
    }

    float xVert = ((float)getX() * m_tileSizeMM);
    float yVert = ((float)getY() * m_tileSizeMM);

    //possible optimisation - we can remove the bottom face of vertices from each inner 
    //tile as they are always up against the outer tile (bar)

    //possible optimisation - we can remove the side face of vertices from both sides 
    //of the active color as they are always up against the blocking inner tile (bar)

    //possible optimisation - we can remove the top face of vertices from the blocking 
    //inner tile as they are always up against the outer tile (bar) bottom vertices

    std::vector<vertices> verticesToReturn = {
        {xVert + xStart,     yVert + yStart,      0},
        {xVert + xEnd,       yVert + yStart,      0},
        {xVert + xEnd,       yVert + yEnd,        0},
        {xVert + xStart,     yVert + yEnd,        0},
        {xVert + xStart,     yVert + yStart,      STENCIL_THICKNESS},
        {xVert + xEnd,       yVert + yStart,      STENCIL_THICKNESS},
        {xVert + xEnd,       yVert + yEnd,        STENCIL_THICKNESS},
        {xVert + xStart,     yVert + yEnd,        STENCIL_THICKNESS},
    };

    return verticesToReturn;
}

std::vector<faces> Tile::getFaces(int & faceStartingNumber, const TileOptions tileOptions) const
{
    std::vector<faces> facesToReturn;
    facesToReturn.push_back({uint32_t(1+faceStartingNumber), uint32_t(2+faceStartingNumber), uint32_t(3+faceStartingNumber), uint32_t(4+faceStartingNumber)});
    facesToReturn.push_back({uint32_t(5+faceStartingNumber), uint32_t(6+faceStartingNumber), uint32_t(7+faceStartingNumber), uint32_t(8+faceStartingNumber)});
    

    // if(tileOptions != TileOptions::omitEnds) //we can omit this face, the outerTile will cover it always
    {
        // std::cout << "Removing ends from a tile" << std::endl;
        facesToReturn.push_back({uint32_t(3+faceStartingNumber), uint32_t(4+faceStartingNumber), uint32_t(8+faceStartingNumber), uint32_t(7+faceStartingNumber)});//top
    }

    // if(tileOptions != TileOptions::omitBottom and tileOptions != TileOptions::omitEnds )
    {
        // std::cout << "Removing bottom or ends from a tile" << std::endl;
        facesToReturn.push_back({uint32_t(1+faceStartingNumber), uint32_t(2+faceStartingNumber), uint32_t(6+faceStartingNumber), uint32_t(5+faceStartingNumber)});//bottom
    }

    // if(tileOptions != TileOptions::omitSides)
    {
        //both sides of the tile
        // std::cout << "Removing both sides from a tile" << std::endl;
        facesToReturn.push_back({uint32_t(2+faceStartingNumber), uint32_t(3+faceStartingNumber), uint32_t(7+faceStartingNumber), uint32_t(6+faceStartingNumber)}); // right side
        facesToReturn.push_back({uint32_t(4+faceStartingNumber), uint32_t(1+faceStartingNumber), uint32_t(5+faceStartingNumber), uint32_t(8+faceStartingNumber)}); // left side    
    }
    

    faceStartingNumber += 8;

    return facesToReturn;
}

std::string Tile::toString() const
{
    std::ostringstream ss;
    ss << "Tile - x:" << getX() << " y:" << getY() << "\n" << m_translatedPixel->toString();
    return ss.str();
}
