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
    printf("tile:  x: %d  y:%d\n",getX(), getY());
    //translate a group of pixels into a single pixel using the 
    //average value of each color
    printf("HERE: \n");
    const Pixel::Colors & colors = getColorArray();
    printf("HERE colors: \n");

    printf("HERE colors: %d \n", colors.size());

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
        float decimalValue = 255 / averageValue;

        uint8_t calculatedValue = (int)round(decimalValue * m_steps); //

        // printf("numberOfPixels %d\n",numberOfPixels);
        // printf("accumaltiveValue %d\n",accumaltiveValue);
        // printf("averageValue %d\n",averageValue);
        // printf("decimalValue %f\n",decimalValue);
        // printf("calculatedValue %d\n",calculatedValue);
        printf("Color %d     is value %d\n", i, calculatedValue);

        m_translatedColors[i] = calculatedValue;


        // return;
    }
}