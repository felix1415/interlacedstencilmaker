#include "utest.h"
#include "../src/Tile.h"
#include "../src/RGBTile.h"
#include "../src/OBJ.h"
#include "../src/Pixel.h"
#include "../src/TranslatedPixel.h"
#include <bitmap_image.hpp>

UTEST(RGBTile, basic) {
    std::unique_ptr<TranslatedPixel> tp;
    Position pos(10,15);

    rgb_t color;
    color.red = 20;
    color.green = 30;
    color.blue = 40;

    Pixel p(10, 15, color);
    std::vector<Pixel> pixels;
    pixels.push_back(p);
    int steps = 5;
    float tileSizeMM = 1.0f;

    tp.reset(new TranslatedPixel(pos, std::move(pixels), steps, tileSizeMM));

    RGBTile t(std::move(tp));

    ASSERT_TRUE(true);
}

UTEST(RGBTile, getOBJData) {
    std::unique_ptr<TranslatedPixel> tp;
    Position pos(10,15);

    rgb_t color;
    color.red = 255;
    color.green = 100;
    color.blue = 0;

    Pixel p(1, 2, color);
    std::vector<Pixel> pixels;
    pixels.push_back(p);
    int steps = 5;
    float tileSizeMM = 1.0f;

    tp.reset(new TranslatedPixel(pos, std::move(pixels), steps, tileSizeMM));

    RGBTile tile(std::move(tp));

    int faceId = 0;

    OBJData tileData = tile.getOBJData(faceId, bitmap_image::color_plane::blue_plane);
    ASSERT_TRUE(true);
}

UTEST(RGBTile, getOBJData2) {    std::unique_ptr<TranslatedPixel> tp;
    Position pos(10,15);

    rgb_t color;
    color.red = 100;
    color.green = 100;
    color.blue = 100;

    Pixel p(1, 4, color);
    std::vector<Pixel> pixels;
    pixels.push_back(p);
    int steps = 5;
    float tileSizeMM = 1.0f;

    tp.reset(new TranslatedPixel(pos, std::move(pixels), steps, tileSizeMM));

    RGBTile tile(std::move(tp));

    int faceId = 0;

    OBJData tileData = tile.getOBJData(faceId, bitmap_image::color_plane::blue_plane);

    //vertices - 1 colour optimised out, so only 8 * 2
    ASSERT_TRUE(tileData.first.size() == 8 * 2);
    //faces - 1 colour optimised out, so only 6 * 2
    ASSERT_TRUE(tileData.second.size() == 6 * 2);
}