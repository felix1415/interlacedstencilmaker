//Copyright (c) 2021 Alex Gray

#pragma once
#include "Pixel.h"
#include "Stencil.h"
#include <bitmap_image.hpp>
#include <iostream>
#include <string>


namespace cics
{
    static const bitmap_image::color_plane colors[] = { bitmap_image::color_plane::blue_plane, 
                                                        bitmap_image::color_plane::green_plane, 
                                                        bitmap_image::color_plane::red_plane };

    static const Stencil::stencilType types[] = { Stencil::stencilType::top, Stencil::stencilType::bottom };

    class Stencilator
    {
        public:
            Stencilator(const uint16_t width, const uint16_t height, const std::string &inputFile, const std::string & outputFile, const bool debug);
            int execute();

        private:
            int calculateSteps(const float tileSizeMM);
            std::pair<float, int> calculatetileSizeAndPixelMultiplier(const bitmap_image & image);
            std::vector<Pixel> getPixels(const int pixelsPerSlot, const bitmap_image & image, int x, int y);

        private:
            const uint16_t m_plateWidth;
            const uint16_t m_plateHeight;
            const float m_nozzleWidth;
            const std::string m_inputFile;
            const std::string m_outputFile;
            const bool m_debug;
    };
}