#include "utest.h"
#include "../src/Tile.h"
#include "../src/OBJ.h"
#include "../src/Pixel.h"
#include "../src/TranslatedPixel.h"
#include <bitmap_image.hpp>

UTEST(Tile, basic) {
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

    Tile t(std::move(tp));

    ASSERT_TRUE(true);
}

UTEST(Tile, basicTwo) {
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

    Tile tile(std::move(tp));

    int faceId = 0;

    OBJData tileData = tile.getOBJData(bitmap_image::color_plane::blue_plane, faceId);
    ASSERT_TRUE(true);
}