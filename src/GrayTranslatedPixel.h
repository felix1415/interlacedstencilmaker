//Copyright (c) 2021 Alex Gray
#pragma once
#include "Pixel.h"
#include "TranslatedPixel.h"

class GrayTranslatedPixel : public TranslatedPixel
{
    public:
        typedef std::array<float, 1> Colors;

    public:
        GrayTranslatedPixel(const Position & position, std::vector<Pixel> && pixels, const uint16_t steps, const float tileSizeMM);
        GrayTranslatedPixel();

        float getTranslatedColorValue(const bitmap_image::color_plane color) const override;
        std::string toString() const override;
        //shadow pixel function
        size_t getColorArraySize() const override;

    private:
        float getColorMultiplier(const bitmap_image::color_plane color);


    private:
        GrayTranslatedPixel::Colors m_translatedColors;
};