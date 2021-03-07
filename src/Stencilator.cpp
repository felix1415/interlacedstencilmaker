//Copyright (c) 2021 Alex Gray

#include "Stencilator.h"
#include "Tile.h"
#include "Position.h"
#include <tuple>
#include <iostream>
#include <sstream>
#include <memory>

namespace cics
{
    Stencilator::Stencilator(const uint16_t width, const uint16_t height, const std::string &inputFile, const std::string & outputFile, const bool debug):
    m_plateWidth(width),
    m_plateHeight(height),
    m_nozzleWidth(0.6f),
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
        int pixelsPerTile = 0;
        Position bounds;
        std::tie(tileSizeMM, pixelsPerTile, bounds) = calculateTileGeometries(image);

        //calculate the number of steps we can take to make a tile smaller/larger
        int steps = calculateSteps(tileSizeMM);

        //we need a certain number of steps to have decent resolution - having 4 is still low
        if(steps < 4)
        {
            printf("Steps value is too low: %d. Lower resolution picture is required.\n", steps);
            return 1;
        }

        //how many pixel per axis do we need to process for each translated pixel
        int pixelsPerAxis = 1;
        if(pixelsPerTile > 1)
        {
            pixelsPerAxis = sqrt(pixelsPerTile);
        }

        if(m_debug)
        {
            printf("tileSizeMM: %f\n",tileSizeMM);
            printf("pixelsPerTile: %d\n",pixelsPerTile);
            printf("steps: %d\n",steps);
        }

        std::vector<Tile> tiles;

        int imageX = 0;
        int imageY = 0;
        int trueY = 0;
        int trueX = 0;

        // bounds = Position(4, 4);
        const int xBoundary = bounds.getX();
        const int yBoundary = bounds.getY();

        while(trueY < yBoundary)
        {
            while(trueX < xBoundary)
            {

                std::vector<Pixel> pixels = getPixels(pixelsPerTile, image, imageX, imageY);
                std::unique_ptr<TranslatedPixel> tp = std::unique_ptr<TranslatedPixel>(new TranslatedPixel(Position(trueX, trueY), std::move(pixels), steps, tileSizeMM));

                if(m_debug)
                {
                    // printf("%s\n", tp->toString().c_str());
                }

                tiles.emplace_back(std::move(tp));
                imageX += pixelsPerAxis;


                trueX++;
            }
            imageX = 0;
            imageY += pixelsPerAxis;

            trueX = 0;
            trueY++;
        }

        std::cout << "imageX: " << imageX << " imageY: " << imageY << std::endl;
        std::cout << "trueX " << trueX << "trueY " << trueY << std::endl;
        std::cout << "xBoundary " << xBoundary << "yBoundary " << yBoundary << std::endl;


        std::vector<Stencil> stencils;
        
        for(const auto color : colors)
        {
            for(const auto type : types)
            {
                Stencil stencil(tiles, color, type, bounds, tileSizeMM);
                stencils.push_back(stencil);
                if(m_debug)
                {
                    printf("%s\n", stencil.toString().c_str());
                }
            }
        }

        stencils[0].process();
        stencils[0].output(m_outputFile);
        // stencils[1].process();
        // stencils[1].output(m_outputFile);
        // stencils[2].process();
        // stencils[2].output(m_outputFile);
        // stencils[4].process();
        // stencils[4].output(m_outputFile);

        return 0;
    }

    std::vector<Pixel> Stencilator::getPixels(const int pixelsPerTile, const bitmap_image & image, int x, int y)
    {
        int pixelDepth = sqrt(pixelsPerTile);
        const int xEndRange = x + pixelDepth;
        const int yEndRange = y + pixelDepth;

        const int oldX = x;

        std::vector<Pixel> pixels;

        while(y < yEndRange)
        {
            while(x < xEndRange)
            {
                rgb_t color;
                image.get_pixel(x, y, color);
                pixels.emplace_back(x, y, color);
                x++;
            }
            x = oldX;
            y++;
        }

        return pixels;
    }

    int Stencilator::calculateSteps(const float tileSizeMM)
    {
        float printerStepsMM = 0.1f;

        return round(tileSizeMM / printerStepsMM);
    }

    std::tuple<float, int, Position> Stencilator::calculateTileGeometries(const bitmap_image & image)
    {
        int imageWidth = image.width();
        int imageHeight = image.height();
        int pixelsPerTile = 1;

        //only reduce resolution down to 25%
        while(pixelsPerTile <= 9)
        {
            //minimum tiles required to fit the whole image on the stencil, we'll also
            //let it round down, we can afford to drop off 1/2 off the side/bottom
            int minTilesRequired = std::max((imageWidth / 2) * 2, (imageHeight / 2) * 2);
            minTilesRequired = minTilesRequired / sqrt(pixelsPerTile);
            float tileSizeMM = (float)m_plateWidth/(float)minTilesRequired;

            if(tileSizeMM < m_nozzleWidth)
            {
                printf("calculated tile width is less nozzle width. tile: %f nozzle %f\n",tileSizeMM, m_nozzleWidth);
                //+1 * new value 1 -> 4 -> 9
                pixelsPerTile += pixelsPerTile;
                pixelsPerTile *= pixelsPerTile;
                continue;
            }

            if(m_debug)
            {
                printf("CALC TILE WIDTH   \n");
                printf("m_plateWidth   %d\n",m_plateWidth);
                printf("m_nozzleWidth   %f\n",m_nozzleWidth);
                printf("image.width()   %d\n",image.width());
                printf("image.height()   %d\n",image.height());
                printf("image.pixels   %d\n",image.height()*image.width());
                printf("tileSizeMM       %f\n",tileSizeMM);
                printf("minTilesRequired  %d\n",minTilesRequired);
            }

            return std::tuple<float, int, Position>(tileSizeMM, pixelsPerTile, Position(imageWidth/sqrt(pixelsPerTile), imageHeight/sqrt(pixelsPerTile)));
        }

        throw std::runtime_error("pixelsPerTile will cause degraded quality, the image is too large.");
    }

}