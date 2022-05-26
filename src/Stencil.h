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
            enum stencilPlate {
                          top = 0,
                          bottom = 1
                      };
    public:
        Stencil(std::vector<Tile> & tiles, const bitmap_image::color_plane color, const Stencil::stencilPlate type, const Position & bounds, const float tileSizeMM);

        void process();
        void output(const std::string outputFile, const bool grayscale = false);

        std::string toString() const;

    private:
        void generateOuterTiles(int & fsNumber);
        void addBar(int & fsNumber, const int x, const int y, const int endX, const int endY);
        std::string stencilPlateToString(const Stencil::stencilPlate typeIn) const;

    private:
        const std::vector<Tile> & m_tiles;
        const bitmap_image::color_plane m_color;
        const Position m_bounds;
        const float m_tileSizeMM;
        const Stencil::stencilPlate m_type;
        std::vector<vertices> m_vertices;
        std::vector<faces> m_faces;
};