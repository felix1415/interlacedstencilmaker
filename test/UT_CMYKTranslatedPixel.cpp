#include "utest.h"
#include "../src/Position.h"
#include "../src/Pixel.h"
#include "../src/CMYKTranslatedPixel.h"
#include "../src/CMYKTile.h"
#include <memory>



// UTEST(CMYKTranslatedPixel, basic125) {
//     std::unique_ptr<TranslatedPixel> tp;
//     Position pos(10,15);

//     rgb_t color;
//     color.cyan = 125;
//     color.magenta = 125;
//     color.yellow = 125;
//     color.key = 125;

//     Pixel p(10, 15, color);

//     std::vector<Pixel> pixels;
//     pixels.push_back(p);

//     int steps = 5;
//     float tileSizeMM = 1.0f;

//     tp.reset(new GrayTranslatedPixel(pos, std::move(pixels), steps, tileSizeMM));

//     ASSERT_EQ(tp->getX(), 10);
//     ASSERT_EQ(tp->getY(), 15);

//     //Values are translated using the steps value
//     ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::red_plane));
//     ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::green_plane));
//     ASSERT_EQ(0.4f, tp->getTranslatedColorValue(bitmap_image::color_plane::blue_plane));
// }

UTEST(CMYKTranslatedPixel, getOBJ) {
    std::unique_ptr<TranslatedPixel> tp;
    Position pos(10,15);

    rgb_t color;
    color.red = 255;
    color.green = 255;
    color.blue = 255;

    Pixel p(10, 15, color);

    std::vector<Pixel> pixels;
    pixels.push_back(p);

    int steps = 5;
    float tileSizeMM = 1.0f;

    tp.reset(new CMYKTranslatedPixel(pos, std::move(pixels), steps, tileSizeMM));

    ASSERT_EQ(tp->getX(), 10);
    ASSERT_EQ(tp->getY(), 15);

    CMYKTile tile(std::move(tp));

    int face = 0;

    tile.getOBJData(face, 0);
    tile.getOBJData(face, 0);
    tile.getOBJData(face, 0);
    tile.getOBJData(face, 0);

    //Values are translated using the steps value
    // ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::red_plane));
    // ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::green_plane));
    // ASSERT_EQ(0.4f, tp->getTranslatedColorValue(bitmap_image::color_plane::blue_plane));
}