//Copyright (c) 2021 Alex Gray

#include "TranslatedPixel.h"
#include "Utils.h"
#include <sstream>

TranslatedPixel::TranslatedPixel():
Pixel(),
m_steps(0),
m_tileSizeMM(0.0f)
{
}

TranslatedPixel::TranslatedPixel(const Position & position, std::vector<Pixel> && pixels, const uint16_t steps, const float tileSizeMM):
Pixel(position), // just use the first one to get the colors array populated
m_translatedColors({0,0,0}),
m_pixels(std::move(pixels)),
m_steps(steps),
m_tileSizeMM(tileSizeMM)
{
    //translate a group of pixels into a single pixel using the 
    //average value of each color

    //prevent division by zero
    if(m_pixels.size() == 0 or m_steps == 0)
    {
        return;
    }

    for(size_t i = 0; i < getColorArraySize(); i++)
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
    }
}

float TranslatedPixel::getTranslatedColorValue(const bitmap_image::color_plane color) const
{
    return m_translatedColors[color];
}

float TranslatedPixel::getTileSizeMM() const
{
    return m_tileSizeMM;
}

uint16_t TranslatedPixel::getSteps() const
{
    return m_steps;
}

size_t TranslatedPixel::getColorArraySize() const
{
    return m_translatedColors.size();
}

std::string TranslatedPixel::toString() const
{
    std::ostringstream ss;

    ss << "TP " << getX() << "," << getY() << " - ";
    for(size_t i = 0; i < m_translatedColors.size(); i++)
    {
        ss << Utils::colorToString(i) << ":"<< m_translatedColors[i] << " ";
    }
    ss << "\n";
    for(const auto & pixel : m_pixels)
    {
        ss << "    " << pixel.toString() << "\n";
    }
    return ss.str();
}