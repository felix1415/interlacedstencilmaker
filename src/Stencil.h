//Copyright (c) 2021 Alex Gray

#include <bitmap_image.hpp>
#include "Tile.h"

class Stencil
{
    public:
            enum stencilType {
                          top = 0,
                          bottom = 1
                      };
    public:
        Stencil(std::vector<Tile> & tiles, const bitmap_image::color_plane color, const Stencil::stencilType type);
        Stencil();

        void process();

        std::string toString() const;

    private:
        std::string stencilTypeToString(const Stencil::stencilType typeIn) const;

    private:
        std::vector<Tile> & m_tiles;
        const bitmap_image::color_plane m_color;
        Stencil::stencilType m_type;
};