//Copyright (c) 2021 Alex Gray
#pragma once
#include "Pixel.h"
#include <bitmap_image.hpp>

class TranslatedPixel : Pixel
{
    public:
        TranslatedPixel(std::vector<Pixel> && pixels, const int steps);
        TranslatedPixel();

    private:
        Pixel::Colors m_translatedColors;
        std::vector<Pixel> m_pixels;
        int m_steps;
};