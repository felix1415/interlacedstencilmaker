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
    int xOuterTilesSize = ((m_bounds.getX() + 2) / 2) * ((m_bounds.getY() + 2) / 2);
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
        OBJData tileData;
        if(m_tiles[i]->getY() % 2 and (stencilPlate::top == m_type))
        {
            tileData = m_tiles[i]->getOBJData(faceStartingNumber, m_color); // top is even tiles
        }
        else if((not(m_tiles[i]->getY() % 2)) and (stencilPlate::bottom == m_type))
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

    int yLength = m_bounds.getY() + (1 + (1 - ((m_bounds.getY() * m_type) % 2)) ) - ((1 - (m_bounds.getY() % 2)) * (1 - m_type));
    int xLength = m_bounds.getX() + (m_bounds.getX() % 2) + 1; // this is static comparatively to y axis

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
    int yLength = m_bounds.getY() + (1 + (1 - ((m_bounds.getY() * m_type) % 2)) ) - ((1 - (m_bounds.getY() % 2)) * (1 - m_type));
    int xLength = m_bounds.getX() + (m_bounds.getX() % 2) + 1; // this is static comparatively to y axis

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
        //y height will stay the same as tileSize, x is dependent on image width
        OuterTile outerTile(x+4, y+4, xLengthNew, 1, m_tileSizeMM);
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

    addBar(fsNumber, BUMP_VALUE+1, BUMP_VALUE, xLength, 1); //top of bottom stencil
    addBar(fsNumber, BUMP_VALUE, BUMP_VALUE, 1, 1); // positional notch
    addBar(fsNumber, BUMP_VALUE, BUMP_VALUE+1, 1, yLength - 1); //add left side y bar
    // ((m_bounds.getY() % 2) + m_type) - 1
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