//Copyright (c) 2021 Alex Gray

#include "Tile.h"
#include <bitmap_image.hpp>
#include <math.h>
#include <iostream>
#include <sstream>

Tile::Tile(std::vector<Pixel> & pixels, const int steps):
m_pixel(std::move(pixels), steps),
m_steps(steps)
{
    // printf("HERE: \n");
}

    // void TranslatedPixel::calculateTileColorValues(const Pixel::Colors & colorMap)
    // {
    //     // for(auto& color : colorMap)
    //     uint8_t numberOfPixels = m_pixels.size();
    //     uint16_t accumaltiveValue = 0;

    //     for(auto& pixel : m_pixels) 
    //     {
    //         accumaltiveValue = getColorValue(pixel.getColor());
    //     }

    //     uint8_t averageValue = (accumaltiveValue / numberOfPixels);
    //     float decimalValue = 127 / averageValue;

    //     uint8_t strength = (int)round(decimalValue * m_steps);

    //     // printf("numberOfPixels %d\n",numberOfPixels);
    //     // printf("accumaltiveValue %d\n",accumaltiveValue);
    //     // printf("averageValue %d\n",averageValue);
    //     // printf("decimalValue %f\n",decimalValue);
    //     // printf("m_strength %d\n",m_strength);
    // }

    // void Tile::getOBJ()
    // {

    // }