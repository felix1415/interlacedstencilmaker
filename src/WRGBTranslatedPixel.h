//Copyright (c) 2021 Alex Gray
#pragma once
#include "Pixel.h"
#include "TranslatedPixel.h"

class WRGBTranslatedPixel : public TranslatedPixel
{
    public:
        typedef std::array<float, 3> Colors;

    public:
        WRGBTranslatedPixel(const Position & position, std::vector<Pixel> && pixels, const uint16_t steps, const float tileSizeMM);
        WRGBTranslatedPixel();

        float getTranslatedColorValue(const bitmap_image::color_plane color) const override;
        //shadow pixel function
        size_t getColorArraySize() const override;

    private:
        WRGBTranslatedPixel::Colors m_translatedColors;
};