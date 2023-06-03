//Copyright (c) 2021 Alex Gray

#pragma once
#include "OBJ.h"
#include "Position.h"
#include "Tile.h"
#include <vector>

class Stencil
{
    public:
            enum stencilPlate {
                          top = 0,
                          bottom = 1
                      };
            static const int BUMP_VALUE = 4;
    public:
        Stencil(const std::vector<std::unique_ptr<Tile>> & tiles, const int color, const Stencil::stencilPlate type, const Position & bounds, const float tileSizeMM, const bool bufferStrips);

        void process();
        void output(const std::string outputFile, const std::string name = "");

        std::string toString() const;

    private:
        void generateBufferStrip(int & fsNumber);
        void generateOuterTiles(int & fsNumber);
        void addBarOfWidth(int & fsNumber, const float x, const float y, const float endX, const float endY, const float width);
        void addBar(int & fsNumber, const int x, const int y, const int endX, const int endY, const float width = 1.0f);
        std::string stencilPlateToString(const Stencil::stencilPlate typeIn) const;

    private:
        const std::vector<std::unique_ptr<Tile>> & m_tiles;
        const int m_color;
        const Position m_bounds;
        const float m_tileSizeMM;
        const Stencil::stencilPlate m_type;
        const bool m_bufferStrips;
        std::vector<vertices> m_vertices;
        std::vector<faces> m_faces;
};

static const Stencil::stencilPlate plates[] = { Stencil::stencilPlate::top, Stencil::stencilPlate::bottom };