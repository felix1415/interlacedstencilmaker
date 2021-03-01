#include "utest.h"
#include "../src/Tile.h"
#include "../src/OBJ.h"
#include "../src/Pixel.h"
#include "../src/TranslatedPixel.h"

UTEST(Tile, basic) {
    TranslatedPixel tp;

    rgb_t color;
    color.red = 20;
    color.green = 30;
    color.blue = 40;

    Pixel p(10, 15, color);
    std::vector<Pixel> pixels;
    pixels.push_back(p);
    int steps = 5;
    float tileSizeMM = 1.0f;

    tp = TranslatedPixel(std::move(pixels), steps, tileSizeMM);

    Tile t(std::move(tp));
}

UTEST(Tile, basic) {
    TranslatedPixel tp;

    rgb_t color;
    color.red = 255;
    color.green = 100;
    color.blue = 0;

    Pixel p(1, 2, color);
    std::vector<Pixel> pixels;
    pixels.push_back(p);
    int steps = 5;
    float tileSizeMM = 1.0f;

    tp = TranslatedPixel(std::move(pixels), steps, tileSizeMM);

    Tile tile(std::move(tp));
    ObjectNumberGenerator ONG();

    OBJData tileData = tile.getOBJData(m_objectNumberGenerator);
}