//Copyright (c) 2021 Alex Gray

#include "GrayTranslatedPixel.h"
#include "Utils.h"
#include <sstream>

GrayTranslatedPixel::GrayTranslatedPixel():
TranslatedPixel()
{
}

GrayTranslatedPixel::GrayTranslatedPixel(const Position & position, std::vector<Pixel> && pixels, const uint16_t steps, const float tileSizeMM):
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

        m_translatedColors[0] += calculatedStrengthValue * getColorMultiplier((bitmap_image::color_plane)i);
    }
}

//this will crash if we try to use anything but 0/blue - it is gray, but it's stored as 'blue'
float GrayTranslatedPixel::getTranslatedColorValue(const bitmap_image::color_plane color) const
{
    return m_translatedColors[color];
}

size_t GrayTranslatedPixel::getColorArraySize() const
{
    return m_translatedColors.size();
}

float GrayTranslatedPixel::getColorMultiplier(const bitmap_image::color_plane color)
{
    switch(color)
    {
        case bitmap_image::color_plane::red_plane:
            return 0.21f;
        case bitmap_image::color_plane::green_plane:
            return 0.72f;
        case bitmap_image::color_plane::blue_plane:
            return 0.07f;
        default:
            throw std::runtime_error("Not a color");
    }
}

std::string GrayTranslatedPixel::toString() const
{
    std::ostringstream ss;

    ss << "TP " << getX() << "," << getY() << " - ";
    for(size_t i = 0; i < m_translatedColors.size(); i++)
    {
        ss << "grayscale" << ":"<< m_translatedColors[i] << " ";
    }
    ss << "\n";
    // for(const auto & pixel : m_pixels)
    // {
    //     ss << "    " << pixel.toString() << "\n";
    // }
    return ss.str();
}

