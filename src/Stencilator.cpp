//Copyright (c) 2021 Alex Gray

#include "Stencilator.h"
#include "Tile.h"
#include "Position.h"
#include <tuple>
#include <iostream>
#include <sstream>

namespace cics
{
    Stencilator::Stencilator(const uint16_t width, const uint16_t height, const std::string &inputFile, const std::string & outputFile, const bool debug):
    m_plateWidth(width),
    m_plateHeight(height),
    m_nozzleWidth(0.4f),
    m_inputFile(inputFile),
    m_outputFile(outputFile),
    m_debug(debug)
    {
    }

    int Stencilator::execute()
    {
        //load image using Arash Partow's Bitmap Image Reader Writer Library 
        bitmap_image image(m_inputFile);
        if (!image)
        {
            printf("Error - Failed to open: input.bmp\n");
            return 1;
        }

        if(m_debug)
        {
            printf("m_plateHeight: %d\n",m_plateHeight);
            printf("m_plateWidth: %d\n",m_plateWidth);
            printf("image.height: %d\n",image.height());
            printf("image.width: %d\n",image.width());
        }

        float tileSizeMM = 0;
        int pixelsPerSlot = 0;
        std::tie(tileSizeMM, pixelsPerSlot) = calculatetileSizeAndPixelMultiplier(image);
        //calculate the number of steps we can take to make a tile smaller/larger
        int steps = calculateSteps(tileSizeMM);

        std::vector<Pixel> pixels;

        if(m_debug)
        {
            printf("tileSizeMM: %f\n",tileSizeMM);
            printf("pixelsPerSlot: %d\n",pixelsPerSlot);
            printf("steps: %d\n",steps);
        }

        // unsigned int total_number_of_pixels = 0;
        int testx = 4;
        int testy = 1;

        std::vector<Tile> tiles;

        int objectNumbers = 0;

        size_t y = 0;
        size_t x = 0;
        while(y < testy)
        {
            while(x < testx)
            {
                std::vector<Pixel> pixels = getPixels(pixelsPerSlot, image, x, y);
                TranslatedPixel tp(std::move(pixels), steps, tileSizeMM);
                Tile tile(std::move(tp));
                tiles.push_back(tile);
                x += pixelsPerSlot;
            }
            y += pixelsPerSlot;
        }


        std::vector<Stencil> stencils;
        Position bounds(testx, testy);
        for(const auto color : colors)
        {
            for(const auto type : types)
            {
                Stencil stencil(tiles, color, type, bounds);
                stencils.push_back(stencil);
                printf("%s\n", stencil.toString().c_str());
            }
        }

        stencils[0].process();



        // for (std::size_t y = 0; y < m_plateHeight; ++y)
        // {
        //     for (std::size_t x = 0; x < m_plateWidth; ++x)
        //     {
        //         std::vector<Pixel> pixels = getPixels(pixelsPerSlot, image, x, y);
        //         Tile tile(pixels, steps);
        //         tiles.push_back(tile);
        //     }
        // }



        // Stencil redStencil()

        //for each colour 
            //for each pixel
                //split into even lines / odd lines
                    //for each pixel
                        //decide pixel colour strength
                        //create a tile for pixel
                        //push tile to vector
                    //use vec of tiles to create Stencil (Adds in the borders)
                    //output stencil to obj file
        

    
        return 0;
    }

    std::vector<Pixel> Stencilator::getPixels(const int pixelsPerSlot, const bitmap_image & image, int x, int y)
    {
        int pixelDepth = sqrt(pixelsPerSlot);
        const int xEndRange = x + pixelDepth;
        const int yEndRange = y + pixelDepth;

        std::vector<Pixel> pixels;

        while(y < yEndRange)
        {
            while(x < xEndRange)
            {
                rgb_t color;
                image.get_pixel(x, y, color);

                Pixel p (x, y, color);
                pixels.push_back(p);
                x++;
            }
            y++;
        }

        return pixels;
    }

    int Stencilator::calculateSteps(const float tileSizeMM)
    {
        float printerStepsMM = 0.1f;

        return (tileSizeMM * 100) / (printerStepsMM * 100);
    }

    std::pair<float, int> Stencilator::calculatetileSizeAndPixelMultiplier(const bitmap_image & image)
    {
        int imageWidth = image.width();
        int imageHeight = image.height();
        int pixelsPerSlot = 1;

        while(pixelsPerSlot < 10)
        {
            //number of slots we could have at m_nozzleWidth gaps
            // 200mm plate -> 0.4mm nozzle = 500px
            int tileSlotsMax = round((float)m_plateWidth / (m_nozzleWidth));
            //slots divided by the number of pixels in width to represent - we need smallest
            float xWidth = (float)tileSlotsMax / (float)imageWidth;
            float yWidth = (float)tileSlotsMax / (float)imageHeight;



            float tileSizeMM = std::min(xWidth, yWidth);

            if(tileSizeMM < m_nozzleWidth)
            {
                printf("calculated tile width is less nozzle width. tile: %f  nozzle %f\n",tileSizeMM, m_nozzleWidth);
                imageWidth = ceil((float)imageWidth / 2.0f);
                imageHeight = ceil((float)imageHeight / 2.0f);
                //+1 * new value 1 -> 4 -> 9
                pixelsPerSlot += pixelsPerSlot;
                pixelsPerSlot *= pixelsPerSlot;
                // (++pixelsPerSlot)*pixelsPerSlot;
            }

            if(m_debug)
            {
                printf("CALC TILE WIDTH   \n");
                printf("xWidth       %f\n",xWidth);
                printf("yWidth       %f\n",yWidth);
                printf("m_plateWidth   %d\n",m_plateWidth);
                printf("m_nozzleWidth   %f\n",m_nozzleWidth);
                printf("image.width()   %d\n",image.width());
                printf("image.width()   %d\n",image.height());
                printf("tileSizeMM       %f\n",tileSizeMM);
                printf("tileSlotsMax  %d\n",tileSlotsMax);
            }

            return std::make_pair(tileSizeMM, pixelsPerSlot);
        }

        throw std::runtime_error("pixelsPerSlot will cause degraded quality, the image is too large.");
    }

}