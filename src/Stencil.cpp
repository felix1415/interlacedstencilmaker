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
    for(const auto & tile : m_tiles)
    {
        OBJData tileData = tile.getOBJData(m_objectNumberGenerator);
    }
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