//Copyright (c) 2022 Alex Gray

#include "CMYKTranslatedPixel.h"
#include "TranslatedPixel.h"
#include "Utils.h"
#include <sstream>
#include <limits>

// https://www.rapidtables.com/convert/color/rgb-to-cmyk.html

float CMYKTranslatedPixel::calcColor(const int color, const float key) const
{
    //1 - 0.78 - 0.22 / 1 - 0.22
    float color_der = std::max<float>(color, std::numeric_limits<float>::min())/255;
    return ((1 - color_der) - key) / ( 1 - key);
}

float CMYKTranslatedPixel::calcKey(const int red, const int green, const int blue) const
{
    float r_der = std::max<float>(red, std::numeric_limits<float>::min()) / 255;
    float g_der = std::max<float>(green, std::numeric_limits<float>::min()) / 255;
    float b_der = std::max<float>(blue, std::numeric_limits<float>::min()) / 255;
    return 1 - std::max<float>(std::max<float>(r_der, g_der), b_der);
}

CMYKTranslatedPixel::CMYKTranslatedPixel(const Position & position, std::vector<Pixel> && pixels, const uint16_t steps, const float tileSizeMM):
TranslatedPixel(position, std::move(pixels), steps, tileSizeMM),
m_translatedColors{0,0,0,0}
{
    //translate a group of pixels into a Cyan Magenta Yellow Key arrangement

    //prevent division by zero
    if(m_pixels.size() == 0 or m_steps == 0)
    {
        return;
    }

    //Translate groups of pixle colors to a single pixel of 3 colors
    TranslatedPixel::Colors accColours = {0.0f, 0.0f, 0.0f};
    for(size_t i = 0; i < accColours.size(); i++)
    {
        uint8_t numberOfPixels = m_pixels.size();
        uint16_t accumaltiveValue = 0;

        //we're packing more than one pixel into a tile, find the 
        //accumaltive value of the pixels for this color
        for(auto& pixel : m_pixels) 
        {
            accumaltiveValue += pixel.getColorValue((bitmap_image::color_plane)i);
        }

        accColours[i] = accumaltiveValue / numberOfPixels;
    }

    //Translate RGB to CMYK
    m_translatedColors[3] = calcKey(accColours[0], accColours[1], accColours[2]);
    m_translatedColors[0] = calcColor(accColours[0], m_translatedColors[3]);
    m_translatedColors[1] = calcColor(accColours[1], m_translatedColors[3]);
    m_translatedColors[2] = calcColor(accColours[2], m_translatedColors[3]);


    for(size_t i = 0; i < getColorArraySize(); i++)
    {
        // float calculatedStrengthValue = m_tileSizeMM * m_translatedColors[i];
        uint8_t calculatedStepValue = (int)round(m_translatedColors[i] * m_steps); 

        float calculatedStrengthValue = (m_tileSizeMM/m_steps) * calculatedStepValue;
        m_translatedColors[i] = calculatedStrengthValue;
        // std::cout << "m_translatedColors[i] i: " << i << " m_translatedColors[i]: " << m_translatedColors[i] << std::endl;
    }

    std::cout << "cyan: " << m_translatedColors[0] << " (" << position.getX() << ", " << position.getY() << ")" << std::endl;
    std::cout << "magenta: " << m_translatedColors[1] << " (" << position.getX() << ", " << position.getY() << ")" << std::endl;
    std::cout << "yellow: " << m_translatedColors[2] << " (" << position.getX() << ", " << position.getY() << ")" << std::endl;
    std::cout << "key: " << m_translatedColors[3] << " (" << position.getX() << ", " << position.getY() << ")" << std::endl;
    std::cout << "" << std::endl;
}

std::string CMYKTranslatedPixel::toString() const
{
    std::ostringstream ss;

    ss << "CMYK TP " << getX() << "," << getY() << " - ";
    for(size_t i = 0; i < m_translatedColors.size(); i++)
    {
        ss << Utils::colorToStringCMYK(i) << ":"<< m_translatedColors[i] << " ";
    }
    ss << "\n";
    for(const auto & pixel : m_pixels)
    {
        ss << "    " << pixel.toString() << "\n";
    }
    return ss.str();
}

float CMYKTranslatedPixel::getTranslatedColorValue(const int color) const
{
    return m_translatedColors[color];
}

size_t CMYKTranslatedPixel::getColorArraySize() const
{
    return m_translatedColors.size();
}

