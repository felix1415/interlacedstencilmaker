//Copyright (c) 2021 Alex Gray

#include "Stencilator.h"
#include "Tile.h"
#include "RGBTile.h"
#include "CMYKTile.h"
#include "GrayTranslatedPixel.h"
#include "CMYKTranslatedPixel.h"
#include "Position.h"
#include "Utils.h"
#include <tuple>
#include <iostream>
#include <sstream>
#include <memory>


Stencilator::Stencilator(const uint16_t width, const uint16_t height, const std::string &inputFile, const std::string & outputFile, const bool info, const bool debug, const StencilType::Type stencilType):
m_plateWidth(width),
m_plateHeight(height),
m_minimumTileSize(1.5f),
m_inputFile(inputFile),
m_outputFile(outputFile),
m_info(info),
m_debug(debug),
m_stencilType(stencilType)
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
    uint16_t steps = calculateSteps(tileSizeMM);

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

    std::vector<std::unique_ptr<Tile>> tiles;

    int imageX = 0;
    int imageY = 0;
    int trueY = 0;
    int trueX = 0;

    // bounds = Position(4, 5);
    // bounds = Position(50,50);

    while(trueY < bounds.getY())
    {
        while(trueX < bounds.getX())
        {
            std::vector<Pixel> pixels = getPixels(pixelsPerTile, image, imageX, imageY);
            std::unique_ptr<TranslatedPixel> tp;

            switch(m_stencilType)
            {
                case StencilType::Type::grayscale:
                    generateTiles<RGBTile, GrayTranslatedPixel>(tiles, std::move(pixels), trueX, trueY, steps, tileSizeMM);
                    break;
                case StencilType::Type::cmyk:
                    generateTiles<CMYKTile, CMYKTranslatedPixel>(tiles, std::move(pixels), trueX, trueY, steps, tileSizeMM);
                    break;
                case StencilType::Type::rgb:
                    generateTiles<RGBTile, TranslatedPixel>(tiles, std::move(pixels), trueX, trueY, steps, tileSizeMM);
                    break;
                default:
                    break;
            }

            imageX += pixelsPerAxis;
            trueX++;
        }
        imageX = 0;
        imageY += pixelsPerAxis;

        trueX = 0;
        trueY++;
    }


    if(m_info)
    {
        std::cout << "imageX: " << imageX << " imageY: " << imageY << std::endl;
        std::cout << "trueX " << trueX << "trueY " << trueY << std::endl;
        std::cout << "xBoundary " << bounds.getX() << "yBoundary " << bounds.getY() << std::endl;
    }


    std::vector<Stencil> stencils;

    for(auto & tile : tiles)
    {
        tile->bump(5);
        if(m_debug)
        {
            std::cout << tile->toString() << std::endl;
        }
    }

    int numberOfColors = StencilType::numberOfColors(m_stencilType);
    bool addBufferStrips = (m_stencilType == StencilType::Type::cmyk);

    for(int color=0; color < numberOfColors; color++)
    {
        for(const auto plate : plates)
        {
            generateStencil(tiles, color, plate, bounds, tileSizeMM, addBufferStrips, StencilType::colorNameFunction(m_stencilType)(color));
        }
    }

    return 0;
}

template <class T, class P>
void Stencilator::generateTiles(Tiles & tiles, std::vector<Pixel> && pixels, const int x, const int y, const int steps, const float tileSizeMM) const
{
    std::unique_ptr<TranslatedPixel> tp = std::unique_ptr<P>(new P(Position(x, y), std::move(pixels), steps, tileSizeMM));
    tiles.emplace_back(std::unique_ptr<T>(new T(std::move(tp))));
}

void Stencilator::generateStencil(const Tiles & tiles, 
                                  const int color, 
                                  const Stencil::stencilPlate plate, 
                                  const Position & bounds, 
                                  const float tileSizeMM, 
                                  const bool bufferStrips,
                                  const std::string name)
{
    Stencil stencil(tiles, color, plate, bounds, tileSizeMM, bufferStrips);
    stencil.process();
    stencil.output(m_outputFile, name);
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

uint16_t Stencilator::calculateSteps(const float tileSizeMM)
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
    while(pixelsPerTile < 150)
    {
        //minimum tiles required to fit the whole image on the stencil, we'll also
        //let it round down, we can afford to drop off 1/2 off the side/bottom
        int minTilesRequired = std::max((imageWidth / 2) * 2, (imageHeight / 2) * 2);
        minTilesRequired = minTilesRequired / sqrt(pixelsPerTile);
        float tileSizeMM = (float)m_plateWidth/(float)minTilesRequired;

        if(tileSizeMM < m_minimumTileSize)
        {
            printf("calculated tile width is less than nozzle width. tile: %f nozzle %f\n",tileSizeMM, m_minimumTileSize);
            //+1 * new value 1 -> 4 -> 9
            pixelsPerTile = sqrt(pixelsPerTile) + 1;
            pixelsPerTile *= pixelsPerTile;
            continue;
        }

        if(m_debug)
        {
            printf("CALC TILE WIDTH   \n");
            printf("m_plateWidth   %d\n",m_plateWidth);
            printf("m_minimumTileSize   %f\n",m_minimumTileSize);
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