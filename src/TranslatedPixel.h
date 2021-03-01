//Copyright (c) 2021 Alex Gray
#pragma once
#include "Pixel.h"
#include <bitmap_image.hpp>
#include <vector>

class TranslatedPixel : public Pixel
{

    typedef std::array<float, 3> Colors;

    public:
        TranslatedPixel(std::vector<Pixel> && pixels, const int steps, const float tileSizeMM);
        TranslatedPixel();

        float getTranslatedColorValue(const bitmap_image::color_plane color) const;
        float getTileSizeMM() const;
        std::string toString() const;

        int getColorValue(const bitmap_image::color_plane color) const = delete;

    private:
        TranslatedPixel::Colors m_translatedColors;
        std::vector<Pixel> m_pixels;
        int m_steps;
        float m_tileSizeMM;
};