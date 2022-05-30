//Copyright (c) 2021 Alex Gray
#pragma once
#include "Pixel.h"
#include "TranslatedPixel.h"

class CMYKTranslatedPixel : public TranslatedPixel
{
    public:
        typedef std::array<float, 4> Colors;

    public:
        CMYKTranslatedPixel(const Position & position, std::vector<Pixel> && pixels, const uint16_t steps, const float tileSizeMM);
        CMYKTranslatedPixel();

        float getTranslatedColorValue(const int color) const override;
        std::string toString() const override;
        //shadow pixel function
        size_t getColorArraySize() const override;

    private:
        float calcColor(const int color, const float key) const;
        float calcKey(const int red, const int green, const int blue) const;

    private:
        CMYKTranslatedPixel::Colors m_translatedColors;
};