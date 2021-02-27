//Copyright (c) 2021 Alex Gray

#include <sstream>
#include "TranslatedPixel.h"
#include "Utils.h"

TranslatedPixel::TranslatedPixel():
Pixel()
{
}

TranslatedPixel::TranslatedPixel(std::vector<Pixel> && pixels, const int steps):
Pixel(pixels[0]), // just use the first one to get the colors array populated
m_pixels(pixels),
m_steps(steps)
{
    //translate a group of pixels into a single pixel using the 
    //average value of each color

    for(size_t i = 0; i < getColorArray().size(); i++)
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

        uint8_t calculatedValue = (int)round(fractionValue * m_steps); 

        

        m_translatedColors[i] = calculatedValue;
    }
    printf("%s\n", toString().c_str());
}

int TranslatedPixel::getColorValue(const bitmap_image::color_plane color) const
{
    return m_translatedColors[color];
}

std::string TranslatedPixel::toString() const
{
    std::ostringstream ss;

    ss << "TP " << getX() << "," << getY() << " - ";
    for(size_t i = 0; i < m_translatedColors.size(); i++)
    {
        ss << Utils::colorToString(i) << ":"<< m_translatedColors[i] << " ";
    }
    return ss.str();
}