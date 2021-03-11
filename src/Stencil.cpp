//Copyright (c) 2021 Alex Gray

#include <sstream>
#include "Stencil.h"
#include "OuterTile.h"
#include "Utils.h"

Stencil::Stencil(std::vector<Tile> & tiles, const bitmap_image::color_plane color, const Stencil::stencilType type, const Position & bounds, const float tileSizeMM, const bool struts):
m_tiles(tiles),
m_color(color),
m_bounds(bounds),
m_tileSizeMM(tileSizeMM),
m_type(type),
m_struts(struts)
{
    int xOuterTilesSize = ((m_bounds.getX() + 2) / 2) * ((m_bounds.getY() + 2) / 2);
    m_vertices.reserve((INNER_TILE_VERTS * m_tiles.size()) + xOuterTilesSize); // 8 vertices * 3 colors
    m_faces.reserve((INNER_TILE_FACES * m_tiles.size()) + xOuterTilesSize);
}

void Stencil::process()
{
    int faceStartingNumber = 0;

    generateOuterTiles(faceStartingNumber);

    if(m_struts)
    {
        // generateStruts(faceStartingNumber);
    }

    for(size_t i = 0; i < m_tiles.size(); i++)
    {
        OBJData tileData;
        if(m_tiles[i].getY() % 2 and (stencilType::top == m_type))
        {
            tileData = m_tiles[i].getOBJData(m_color, faceStartingNumber); // top is even tiles
        }
        else if(not(m_tiles[i].getY() % 2) and (stencilType::bottom == m_type))
        {
            tileData = m_tiles[i].getOBJData(m_color, faceStartingNumber); //bottom is odd tiles
        }

        m_vertices.insert(std::end(m_vertices), std::begin(tileData.first), std::end(tileData.first));

        m_faces.insert(std::end(m_faces), std::begin(tileData.second), std::end(tileData.second));
    }
}

void Stencil::generateOuterTiles(int & fsNumber)
{
    //while this function was supposed to generate tiles, these are now instead of tiles. 
    //this reduces the number of vertices as well as generation resources
    int numberOfOuterTiles = 0;

    //we might be the tile without the last row of pixels, so we don't want 
    //to put an extra row of outertiles - it'll be unnecessary
    int yLength = m_bounds.getY() + (m_bounds.getY() % 2) + 2;
    int xLength = m_bounds.getX() + (m_bounds.getX() % 2) + 1; // this is static comparatively to y axis

    //we need to create horizontal lines inbetween each row of inner tiles
    for(int y = 0; y < yLength; y++)
    {
        int x = 0;
        if(y == 0)
        {
            x = 1;
        }
        //y height will stay the same as tileSize, x is dependent on image width
        OuterTile outerTile(x, y, xLength, 1, m_tileSizeMM);
        OBJData tileData;

        //we will do the same conditional as in process as we are 
        //shifted by one tile to the left compared to inner tiles 
        if(not (y % 2) and (stencilType::top == m_type) and y != (yLength -1))
        {
            tileData = outerTile.getOBJData(fsNumber); // top is even tiles
        }
        else if((y % 2) and (stencilType::bottom == m_type))
        {
            tileData = outerTile.getOBJData(fsNumber); //bottom is odd tiles
        }
        else if(y == 0 and (stencilType::bottom == m_type))
        {
            tileData = outerTile.getOBJData(fsNumber); //top of bottom stencil
        }
        else
        {
            continue; // a line with inner tiles on, lets go to the next line
        }

        //cura can only handle 65k vertices by the looks of it, anything after 65k defaults to 0,0...
        //outer tiles should be struts and not tiles now
        m_vertices.insert(std::end(m_vertices), std::begin(tileData.first), std::end(tileData.first));

        m_faces.insert(std::end(m_faces), std::begin(tileData.second), std::end(tileData.second));
        ++numberOfOuterTiles;
    }

    {   //add left side y bar
        OuterTile outerTile(0, 1, 1, m_bounds.getY(), m_tileSizeMM);
        OBJData tileData = outerTile.getOBJData(fsNumber); 

        m_vertices.insert(std::end(m_vertices), std::begin(tileData.first), std::end(tileData.first));
        m_faces.insert(std::end(m_faces), std::begin(tileData.second), std::end(tileData.second));
        ++numberOfOuterTiles;
    }

    {   //add positional notch
        OuterTile outerTile(0, 0, 1, 1, m_tileSizeMM);
        OBJData tileData = outerTile.getOBJData(fsNumber); 

        m_vertices.insert(std::end(m_vertices), std::begin(tileData.first), std::end(tileData.first));
        m_faces.insert(std::end(m_faces), std::begin(tileData.second), std::end(tileData.second));
        ++numberOfOuterTiles;
    }

    {   //add right side y bar
        OuterTile outerTile(xLength, 0, 1, m_bounds.getY() + 1 + m_type, m_tileSizeMM);
        OBJData tileData = outerTile.getOBJData(fsNumber); 

        m_vertices.insert(std::end(m_vertices), std::begin(tileData.first), std::end(tileData.first));
        m_faces.insert(std::end(m_faces), std::begin(tileData.second), std::end(tileData.second));
        ++numberOfOuterTiles;
    }


}

void Stencil::output(const std::string outputFile)
{
    std::ostringstream ss;
    ss << outputFile << "_" << stencilTypeToString(m_type) << "_" << Utils::colorToString(m_color) << ".obj";

    OBJ::writeOBJFile(ss.str(), m_vertices, m_faces);
}

std::string Stencil::toString() const
{
    std::ostringstream ss;

    ss << Utils::colorToString(m_color) << " stencil " << stencilTypeToString(m_type) << " - size:" << m_tiles.size();
    return ss.str();
}

std::string Stencil::stencilTypeToString(const Stencil::stencilType typeIn) const
{
    if(typeIn == Stencil::stencilType::top)
        return "top";
    if(typeIn == Stencil::stencilType::bottom)
        return "bottom";

    return "unknown stencil type";
}