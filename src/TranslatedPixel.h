//Copyright (c) 2021 Alex Gray
#pragma once
#include "Pixel.h"
#include <bitmap_image.hpp>

class TranslatedPixel : public Pixel
{
    public:
        TranslatedPixel(std::vector<Pixel> && pixels, const int steps);
        TranslatedPixel();

        int getColorValue(const bitmap_image::color_plane color) const override;

    private:
        Pixel::Colors m_translatedColors;
        std::vector<Pixel> m_pixels;
        int m_steps;
};