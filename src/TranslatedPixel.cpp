//Copyright (c) 2021 Alex Gray

#include "TranslatedPixel.h"

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
    const Pixel::Colors & colors = getColorArray();


    // for(auto& color : colors)
    for(size_t i = 0; i < colors.size(); i++)
    {
        uint8_t numberOfPixels = m_pixels.size();
        uint16_t accumaltiveValue = 0;

        //we're packing more than one pixel into a tile, find the 
        //accumaltive value of the pixels for this color
        for(auto& pixel : m_pixels) 
        {
            accumaltiveValue += pixel.getColorValue((bitmap_image::color_plane)i);
        }
        printf("m_pixels %d     accum value %d\n", m_pixels.size(), accumaltiveValue);

        //
        float averageValue = (float)accumaltiveValue / numberOfPixels;
        float fractionValue = averageValue / 255;

        uint8_t calculatedValue = (int)round(fractionValue * m_steps); 

        // printf("numberOfPixels %d\n",numberOfPixels);
        // printf("accumaltiveValue %d\n",accumaltiveValue);
        // printf("averageValue %d\n",averageValue);
        // printf("fractionValue %f\n",fractionValue);
        // printf("calculatedValue %d\n",calculatedValue);
        printf("Color %d     is value %d\n", i, calculatedValue);

        m_translatedColors[i] = calculatedValue;
    }
}

int TranslatedPixel::getColorValue(const bitmap_image::color_plane color) const
{
    return m_translatedColors[color];
}