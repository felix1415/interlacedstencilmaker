//Copyright (c) 2021 Alex Gray

#include "Tile.h"
#include <bitmap_image.hpp>
#include <math.h>
#include <iostream>
#include <sstream>

namespace cics
{
    Tile::Tile(const std::vector<Pixel> & pixels, const bitmap_image::color_plane color):
    m_pixels(std::move(pixels)),
    m_color(color)
    {
        calculateStrength();
    }

    void Tile::calculateStrength()
    {
        uint8_t numberOfPixels = m_pixels.size();
        uint16_t accumaltiveValue = 0;

        for(auto& pixel : m_pixels) 
        {
            accumaltiveValue = getColorValue(pixel.getColor());
        }

        uint8_t averageValue = (accumaltiveValue / numberOfPixels);
        float decimalValue = 127 / averageValue;

        m_strength = (int)round(decimalValue * Tile::STEPS);

        // printf("numberOfPixels %d\n",numberOfPixels);
        // printf("accumaltiveValue %d\n",accumaltiveValue);
        // printf("averageValue %d\n",averageValue);
        // printf("decimalValue %f\n",decimalValue);
        // printf("m_strength %d\n",m_strength);
    }

    uint8_t Tile::getColorValue(const rgb_t colourObj)
    {
        //we can't cycle over rgb_t values, so we'll have to do conditionals
        if(m_color == bitmap_image::color_plane::red_plane)
            return (uint8_t)colourObj.red;

        if(m_color == bitmap_image::color_plane::green_plane)
            return (uint8_t)colourObj.green;

        if(m_color == bitmap_image::color_plane::blue_plane)
            return (uint8_t)colourObj.blue;

        throw std::runtime_error("color code not defined");
    }

    void Tile::getOBJ()
    {

    }

}