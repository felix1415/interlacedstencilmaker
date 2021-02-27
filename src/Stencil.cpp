//Copyright (c) 2021 Alex Gray

#include <sstream>
#include "Stencil.h"
#include "Utils.h"

Stencil::Stencil(std::vector<Tile> & tiles, const bitmap_image::color_plane color, const Stencil::stencilType type):
m_tiles(tiles),
m_color(color),
m_type(type)
{

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