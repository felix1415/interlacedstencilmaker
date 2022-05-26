//Copyright (c) 2021 Alex Gray

#pragma once
#include "Pixel.h"
#include "Stencil.h"
#include "StencilType.h"
#include <bitmap_image.hpp>
#include <iostream>
#include <string>
#include <vector>

static const Stencil::stencilPlate plates[] = { Stencil::stencilPlate::top, Stencil::stencilPlate::bottom };

class Stencilator
{
    public:
        Stencilator(const uint16_t width, const uint16_t height, const std::string &inputFile, const std::string & outputFile, const bool debug, const StencilType::Type stencilType);
        int execute();

    private:
        uint16_t calculateSteps(const float tileSizeMM);
        std::tuple<float, int, Position> calculateTileGeometries(const bitmap_image & image);
        std::vector<Pixel> getPixels(const int pixelsPerSlot, const bitmap_image & image, int x, int y);

    private:
        const uint16_t m_plateWidth;
        const uint16_t m_plateHeight;
        const float m_minimumTileSize;
        const std::string m_inputFile;
        const std::string m_outputFile;
        const bool m_debug;
        const StencilType::Type m_stencilType;
};