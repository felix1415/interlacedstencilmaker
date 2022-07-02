//Copyright (c) 2021 Alex Gray

#include <sstream>
#include "Stencil.h"
#include "OuterTile.h"
#include "Utils.h"

Stencil::Stencil(std::vector<std::unique_ptr<Tile>> & tiles, const int color, const Stencil::stencilPlate type, const Position & bounds, const float tileSizeMM, const bool bufferStrips):
m_tiles(tiles),
m_color(color),
m_bounds(bounds),
m_tileSizeMM(tileSizeMM),
m_type(type),
m_bufferStrips(bufferStrips)
{
    int xBounds = m_bounds.getX();
    int xOuterTilesSize = ((xBounds + 2) / 2) * ((xBounds + 2) / 2);
    m_vertices.reserve((INNER_TILE_VERTS * m_tiles.size()) + xOuterTilesSize); // 8 vertices * 3 colors
    m_faces.reserve((INNER_TILE_FACES * m_tiles.size()) + xOuterTilesSize);
}

void Stencil::process()
{
    int faceStartingNumber = 0;

    generateOuterTiles(faceStartingNumber);

    if(m_bufferStrips)
        generateBufferStrip(faceStartingNumber);

    for(size_t i = 0; i < m_tiles.size(); i++)
    {
        int y = m_tiles[i]->getY();
        OBJData tileData;
        if(y % 2 and (stencilPlate::top == m_type))
        {
            tileData = m_tiles[i]->getOBJData(faceStartingNumber, m_color); // top is even tiles
        }
        else if((not(y % 2)) and (stencilPlate::bottom == m_type))
        {
            tileData = m_tiles[i]->getOBJData(faceStartingNumber, m_color); //bottom is odd tiles
        }

        m_vertices.insert(std::end(m_vertices), std::begin(tileData.first), std::end(tileData.first));

        m_faces.insert(std::end(m_faces), std::begin(tileData.second), std::end(tileData.second));
    }
}

void Stencil::generateBufferStrip(int & fsNumber)
{
    const float width = 0.5f;

    int yBounds = m_bounds.getY();
    int xBounds = m_bounds.getX();
    int yLength = yBounds + (1 + (1 - ((yBounds * m_type) % 2)) ) - ((1 - (yBounds % 2)) * (1 - m_type));
    int xLength = xBounds + (xBounds % 2) + 1; // this is static comparatively to y axis

    switch(m_color)
    {
        case 0:
            addBarOfWidth(fsNumber, BUMP_VALUE + 1, BUMP_VALUE - 0.5, xLength, 1, width); //top of stencil
            break;
        case 1:
            addBarOfWidth(fsNumber, BUMP_VALUE, 3.5 + yLength, xLength + 1, 1, width); //bottom of stencil
            break;
        case 2:
            addBarOfWidth(fsNumber, xLength + BUMP_VALUE + 0.5, BUMP_VALUE, 1, yLength, width); //add right side y bar
            break;
        case 3:
            addBarOfWidth(fsNumber, BUMP_VALUE - 0.5, BUMP_VALUE + 1, 1, yLength - 1, width); //add left side y bar
            break;
    }
}

void Stencil::generateOuterTiles(int & fsNumber)
{
    //while this function was supposed to generate tiles, these are now instead of tiles. 
    //this reduces the number of vertices as well as generation resources

    //we might be the tile without the last row of pixels, so we don't want 
    //to put an extra row of outertiles - it'll be unnecessary
    int yBounds = m_bounds.getY();
    int xBounds = m_bounds.getX();
    int yLength = yBounds + (1 + (1 - ((yBounds * m_type) % 2)) ) - ((1 - (yBounds % 2)) * (1 - m_type));
    int xLength = xBounds + (xBounds % 2) + 1; // this is static comparatively to y axis

    float upperBufferVal = 0;
    float lowerBufferVal = 0;

    //we need to create horizontal lines inbetween each row of inner tiles
    for(int y = 0; y < yLength; y++)
    {
        int x = 0;
        int xLengthNew = xLength;
        if(y == 0)
        {
            x = 1;
            xLengthNew -= 1;
        }

        if(y == 1 or ((y == 0) and stencilPlate::top == m_type))
        {
            lowerBufferVal = Utils::getBufferOBJValue(m_tileSizeMM);
        }
        else if(y > 1 and not (y >= (yLength-1)))
        {
            lowerBufferVal = Utils::getBufferOBJValue(m_tileSizeMM);
            upperBufferVal = Utils::getBufferOBJValue(m_tileSizeMM);
        }
        else if(y == (yLength-1))
        {
            lowerBufferVal = 0;
        }

        if(m_color == 0 or m_color == 3)
        {
            lowerBufferVal = 0;
            upperBufferVal = 0;
        }

        //y height will stay the same as tileSize, x is dependent on image width
        OuterTile outerTile(x+BUMP_VALUE, y+BUMP_VALUE + (upperBufferVal / m_tileSizeMM), xLengthNew, 1 - ((upperBufferVal + lowerBufferVal) / m_tileSizeMM), m_tileSizeMM);
        OBJData tileData;

        //we will do the same conditional as in process as we are 
        //shifted by one tile to the left compared to inner tiles 
        if(not (y % 2) and (stencilPlate::top == m_type))
        {
            tileData = outerTile.getOBJData(fsNumber); // top is even tiles
        }
        else if((y % 2) and (stencilPlate::bottom == m_type))
        {
            tileData = outerTile.getOBJData(fsNumber); //bottom is odd tiles
        }
        else
        {
            continue; // a line with inner tiles on, lets go to the next line
        }

        m_vertices.insert(std::end(m_vertices), std::begin(tileData.first), std::end(tileData.first));

        m_faces.insert(std::end(m_faces), std::begin(tileData.second), std::end(tileData.second));
    }

    //we only need to take the top off the top plate bar
    lowerBufferVal = m_type ? 0 : Utils::getBufferOBJValue(m_tileSizeMM);

    addBar(fsNumber, BUMP_VALUE+1, BUMP_VALUE, xLength, 1 - lowerBufferVal); //top of bottom stencil
    addBar(fsNumber, BUMP_VALUE, BUMP_VALUE, 1, 1); // positional notch
    addBar(fsNumber, BUMP_VALUE, BUMP_VALUE+1, 1, yLength - 1); //add left side y bar
    addBar(fsNumber, xLength + BUMP_VALUE, BUMP_VALUE, 1, yLength); //add right side y bar
}

void Stencil::addBar(int & fsNumber, const int x, const int y, const int endX, const int endY, const float width)
{
    OuterTile outerTile(x,  y, endX, endY, m_tileSizeMM * width);
    OBJData tileData = outerTile.getOBJData(fsNumber); 

    m_vertices.insert(std::end(m_vertices), std::begin(tileData.first), std::end(tileData.first));
    m_faces.insert(std::end(m_faces), std::begin(tileData.second), std::end(tileData.second));
}

void Stencil::addBarOfWidth(int & fsNumber, const float x, const float y, const float endX, const float endY, const float width)
{
    return addBar(fsNumber, x / width, y / width, endX / width, endY / width,  width); 
}

void Stencil::output(const std::string outputFile, const std::string name)
{
    std::ostringstream ss;
    if(name.compare(""))
        ss << outputFile << "_" << stencilPlateToString(m_type) << "_" << name << ".obj";
    else
        ss << outputFile << "_" << stencilPlateToString(m_type) << "_" << Utils::colorToStringRGB(m_color) << ".obj";

    OBJ::writeOBJFile(ss.str(), m_vertices, m_faces);
}

std::string Stencil::toString() const
{
    std::ostringstream ss;

    ss << Utils::colorToStringRGB(m_color) << " stencil " << stencilPlateToString(m_type) << " - size:" << m_tiles.size();
    return ss.str();
}

std::string Stencil::stencilPlateToString(const Stencil::stencilPlate typeIn) const
{
    if(typeIn == Stencil::stencilPlate::top)
        return "top";
    if(typeIn == Stencil::stencilPlate::bottom)
        return "bottom";

    return "unknown stencil type";
}