//Copyright (c) 2021 Alex Gray

#include <sstream>
#include "Stencil.h"
#include "Utils.h"

Stencil::Stencil(std::vector<Tile> & tiles, const bitmap_image::color_plane color, const Stencil::stencilType type, const Position & bounds):
m_tiles(tiles),
m_color(color),
m_type(type)
{
    int xOuterTilesSize = ((bounds.getX() + 2) / 2) * ((bounds.getY() + 2) / 2);
    m_vertices.reserve((INNER_TILE_VERTS * m_tiles.size()) + xOuterTilesSize); // 8 vertices * 3 colors
    m_faces.reserve((INNER_TILE_FACES * m_tiles.size()) + xOuterTilesSize);
}

void Stencil::process()
{
    int faceStartingNumber = 0;
    // for(const auto & tile : m_tiles)
    for(size_t i = 0; i < m_tiles.size(); i++)
    {
        OBJData tileData;
        // if(m_tiles[i].getY() % (stencilType::top + 1))
        {
            tileData = m_tiles[i].getOBJData(m_color, faceStartingNumber); // top is even tiles
        }

        m_vertices.insert(std::end(m_vertices), std::begin(tileData.first), std::end(tileData.first));

        m_faces.insert(std::end(m_faces), std::begin(tileData.second), std::end(tileData.second));
    }

    //add outer tiles
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