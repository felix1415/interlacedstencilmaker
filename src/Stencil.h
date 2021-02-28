//Copyright (c) 2021 Alex Gray

#pragma once
#include <bitmap_image.hpp>
#include "ObjectNumberGenerator.h"
#include "OBJ.h"
#include "Position.h"
#include "Tile.h"

class Stencil
{
    public:
            enum stencilType {
                          top = 0,
                          bottom = 1
                      };
    public:
        Stencil(std::vector<Tile> & tiles, const bitmap_image::color_plane color, const Stencil::stencilType type, const Position & bounds);

        void process();

        std::string toString() const;

    private:
        std::string stencilTypeToString(const Stencil::stencilType typeIn) const;

    private:
        std::vector<Tile> & m_tiles;
        const bitmap_image::color_plane m_color;
        Stencil::stencilType m_type;
        ObjectNumberGenerator m_objectNumberGenerator;
        std::vector<vertices> m_vertices;
        std::vector<faces> m_faces;
};