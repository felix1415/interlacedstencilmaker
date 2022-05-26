#include "utest.h"
#include "../src/Position.h"
#include "../src/Pixel.h"
#include "../src/GrayTranslatedPixel.h"
#include <memory>

UTEST(GrayTranslatedPixel, basicAll) {
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

    tp.reset(new GrayTranslatedPixel(pos, std::move(pixels), steps, tileSizeMM));

    ASSERT_EQ(tp->getX(), 10);
    ASSERT_EQ(tp->getY(), 15);

    //Values are translated using the steps value
    ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::red_plane));
    ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::green_plane));
    ASSERT_EQ(1.0f, tp->getTranslatedColorValue(bitmap_image::color_plane::blue_plane));
}

UTEST(GrayTranslatedPixel, red) {
    std::unique_ptr<TranslatedPixel> tp;
    Position pos(10,15);

    rgb_t color;
    color.red = 255;
    color.green = 0;
    color.blue = 0;

    Pixel p(10, 15, color);

    std::vector<Pixel> pixels;
    pixels.push_back(p);

    int steps = 5;
    float tileSizeMM = 1.0f;

    tp.reset(new GrayTranslatedPixel(pos, std::move(pixels), steps, tileSizeMM));

    ASSERT_EQ(tp->getX(), 10);
    ASSERT_EQ(tp->getY(), 15);

    //Values are translated using the steps value
    ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::red_plane));
    ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::green_plane));
    ASSERT_EQ(0.21f, tp->getTranslatedColorValue(bitmap_image::color_plane::blue_plane));
}

UTEST(GrayTranslatedPixel, green) {
    std::unique_ptr<TranslatedPixel> tp;
    Position pos(10,15);

    rgb_t color;
    color.red = 0;
    color.green = 255;
    color.blue = 0;

    Pixel p(10, 15, color);

    std::vector<Pixel> pixels;
    pixels.push_back(p);

    int steps = 5;
    float tileSizeMM = 1.0f;

    tp.reset(new GrayTranslatedPixel(pos, std::move(pixels), steps, tileSizeMM));

    ASSERT_EQ(tp->getX(), 10);
    ASSERT_EQ(tp->getY(), 15);

    //Values are translated using the steps value
    ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::red_plane));
    ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::green_plane));
    ASSERT_EQ(0.72f, tp->getTranslatedColorValue(bitmap_image::color_plane::blue_plane));
}

UTEST(GrayTranslatedPixel, blue) {
    std::unique_ptr<TranslatedPixel> tp;
    Position pos(10,15);

    rgb_t color;
    color.red = 0;
    color.green = 0;
    color.blue = 255;

    Pixel p(10, 15, color);

    std::vector<Pixel> pixels;
    pixels.push_back(p);

    int steps = 5;
    float tileSizeMM = 1.0f;

    tp.reset(new GrayTranslatedPixel(pos, std::move(pixels), steps, tileSizeMM));

    ASSERT_EQ(tp->getX(), 10);
    ASSERT_EQ(tp->getY(), 15);

    //Values are translated using the steps value
    ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::red_plane));
    ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::green_plane));
    ASSERT_EQ(0.07f, tp->getTranslatedColorValue(bitmap_image::color_plane::blue_plane));

}

UTEST(GrayTranslatedPixel, basic125) {
    std::unique_ptr<TranslatedPixel> tp;
    Position pos(10,15);

    rgb_t color;
    color.red = 125;
    color.green = 125;
    color.blue = 125;

    Pixel p(10, 15, color);

    std::vector<Pixel> pixels;
    pixels.push_back(p);

    int steps = 5;
    float tileSizeMM = 1.0f;

    tp.reset(new GrayTranslatedPixel(pos, std::move(pixels), steps, tileSizeMM));

    ASSERT_EQ(tp->getX(), 10);
    ASSERT_EQ(tp->getY(), 15);

    //Values are translated using the steps value
    ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::red_plane));
    ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::green_plane));
    ASSERT_EQ(0.4f, tp->getTranslatedColorValue(bitmap_image::color_plane::blue_plane));
}