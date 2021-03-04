//Copyright (c) 2021 Alex Gray

#include <sstream>
#include "Stencil.h"
#include "OuterTile.h"
#include "Utils.h"

Stencil::Stencil(std::vector<Tile> & tiles, const bitmap_image::color_plane color, const Stencil::stencilType type, const Position & bounds, const float tileSizeMM):
m_tiles(tiles),
m_color(color),
m_bounds(bounds),
m_tileSizeMM(tileSizeMM),
m_type(type)
{
    int xOuterTilesSize = ((m_bounds.getX() + 2) / 2) * ((m_bounds.getY() + 2) / 2);
    m_vertices.reserve((INNER_TILE_VERTS * m_tiles.size()) + xOuterTilesSize); // 8 vertices * 3 colors
    m_faces.reserve((INNER_TILE_FACES * m_tiles.size()) + xOuterTilesSize);
}

void Stencil::process()
{
    int faceStartingNumber = 0;

    generateOuterTiles(faceStartingNumber);

    std::cout << "TILE SIZE: " << m_tiles.size() << std::endl;
    for(size_t i = 0; i < m_tiles.size(); i++)
    {
        OBJData tileData;
        if(m_tiles[i].getY() % 2 and (stencilType::top == m_type))
        {
            std::cout << m_tiles[i].toString() << std::endl;
            tileData = m_tiles[i].getOBJData(m_color, faceStartingNumber); // top is even tiles
        }
        else if(not(m_tiles[i].getY() % 2) and (stencilType::bottom == m_type))
        {
            std::cout << "inner bottom tiledata" << std::endl;
            tileData = m_tiles[i].getOBJData(m_color, faceStartingNumber); //bottom is odd tiles
        }

        m_vertices.insert(std::end(m_vertices), std::begin(tileData.first), std::end(tileData.first));

        m_faces.insert(std::end(m_faces), std::begin(tileData.second), std::end(tileData.second));
    }

    //add outer tiles
}

void Stencil::generateOuterTiles(int & fsNumber)
{
    //we start a 0, inner tiles start at 1,1, we finish 1 past bounds 
    //so we have a right hand side of tiles
    for(int x = 0; x < m_bounds.getX() + 1; x++)
    {
        for(int y = 0; y < m_bounds.getY() + 2; y++)
        {
            OuterTile outerTile(x, y, m_tileSizeMM);
            OBJData tileData;

            //we will do the same conditional as in process as we are 
            //shifted by one tile to the left compared to inner tiles 
            if(not (y % 2) and (stencilType::top == m_type))
            {
                tileData = outerTile.getOBJData(fsNumber); // top is even tiles
            }
            else if((y % 2) and (stencilType::bottom == m_type))
            {
                tileData = outerTile.getOBJData(fsNumber); //bottom is odd tiles
            }
            else if(x == 0 or x == m_bounds.getX() + 1)
            {
                tileData = outerTile.getOBJData(fsNumber); //x side
            }
            else if(y == 0 or y == m_bounds.getY() + 1)
            {
                std::cout << "tiledata y is top/bottom" << std::endl;
                tileData = outerTile.getOBJData(fsNumber); //bottom is odd tiles
            }

            m_vertices.insert(std::end(m_vertices), std::begin(tileData.first), std::end(tileData.first));

            m_faces.insert(std::end(m_faces), std::begin(tileData.second), std::end(tileData.second));
        }
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