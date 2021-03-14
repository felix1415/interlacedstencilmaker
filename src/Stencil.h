//Copyright (c) 2021 Alex Gray

#pragma once
#include "OBJ.h"
#include "Position.h"
#include "Tile.h"
#include <bitmap_image.hpp>
#include <vector>

class Stencil
{
    public:
            enum stencilType {
                          top = 0,
                          bottom = 1
                      };
    public:
        Stencil(std::vector<Tile> & tiles, const bitmap_image::color_plane color, const Stencil::stencilType type, const Position & bounds, const float tileSizeMM, const bool struts);

        void process();
        void output(const std::string outputFile);

        std::string toString() const;

    private:
        void generateOuterTiles(int & fsNumber);
        void addBar(int & fsNumber, const int x, const int y, const int endX, const int endY);
        std::string stencilTypeToString(const Stencil::stencilType typeIn) const;

    private:
        const std::vector<Tile> & m_tiles;
        const bitmap_image::color_plane m_color;
        const Position m_bounds;
        const float m_tileSizeMM;
        const Stencil::stencilType m_type;
        const bool m_struts;
        std::vector<vertices> m_vertices;
        std::vector<faces> m_faces;
};