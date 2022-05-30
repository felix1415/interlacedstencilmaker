//Copyright (c) 2021 Alex Gray

#include "WRGBTranslatedPixel.h"
#include "Utils.h"
#include <sstream>
#include <limits>

WRGBTranslatedPixel::WRGBTranslatedPixel():
TranslatedPixel()
{
}

WRGBTranslatedPixel::WRGBTranslatedPixel(const Position & position, std::vector<Pixel> && pixels, const uint16_t steps, const float tileSizeMM):
TranslatedPixel(position, std::move(pixels), steps, tileSizeMM),
m_translatedColors{0}
{
    //translate a group of pixels into a single pixel using the 
    //average value of each color and the luminosity method to 
    //get a grayscale pixel

    //prevent division by zero
    if(m_pixels.size() == 0 or m_steps == 0)
    {
        return;
    }

    float minValue = std::numeric_limits<float>::max();

    for(size_t i = 0; i < Pixel::getColorArray().size(); i++)
    {
        uint8_t numberOfPixels = m_pixels.size();
        uint16_t accumaltiveValue = 0;

        //we're packing more than one pixel into a tile, find the 
        //accumaltive value of the pixels for this color
        for(auto& pixel : m_pixels) 
        {
            accumaltiveValue += pixel.getColorValue((bitmap_image::color_plane)i);
        }

        float averageValue = (float)accumaltiveValue / numberOfPixels;
        float fractionValue = averageValue / 255;

        uint8_t calculatedStepValue = (int)round(fractionValue * m_steps); 

        float calculatedStrengthValue = (m_tileSizeMM/m_steps) * calculatedStepValue;

        m_translatedColors[i] = calculatedStrengthValue;

        if(calculatedStrengthValue < minValue)
        {
            minValue = calculatedStrengthValue;
        }
    }

    for(size_t i = 0; i < Pixel::getColorArray().size(); i++)
    {
        m_translatedColors[i] -= minValue;
        m_translatedColors[i] = m_translatedColors[i] * 1.25;
    }
}

float WRGBTranslatedPixel::getTranslatedColorValue(const int color) const
{
    return m_translatedColors[color];
}

size_t WRGBTranslatedPixel::getColorArraySize() const
{
    return m_translatedColors.size();
}

