#include "utest.h"
#include "../src/Pixel.h"
#include "../src/TranslatedPixel.h"

UTEST(TranslatedPixel, basic) {
    TranslatedPixel tp;

    rgb_t color;
    color.red = 20;
    color.green = 30;
    color.blue = 40;

    Pixel p(10, 15, color);

    std::vector<Pixel> pixels;
    pixels.push_back(p);

    tp = TranslatedPixel(std::move(pixels), 5);

    ASSERT_EQ(tp.getX(), 10);
    ASSERT_EQ(tp.getY(), 15);

    //Values are translated using the steps value
    ASSERT_NE(20, tp.getColorValue(bitmap_image::color_plane::red_plane));
    ASSERT_NE(30, tp.getColorValue(bitmap_image::color_plane::green_plane));
    ASSERT_NE(40, tp.getColorValue(bitmap_image::color_plane::blue_plane));
}

UTEST(TranslatedPixel, basicEmpty) {
    TranslatedPixel tp;

    rgb_t color;
    color.red = 0;
    color.green = 0;
    color.blue = 0;

    Pixel p(0, 0, color);
    std::vector<Pixel> pixels;
    pixels.push_back(p);

    tp = TranslatedPixel(std::move(pixels), 5);

    ASSERT_EQ(tp.getX(), 0);
    ASSERT_EQ(tp.getY(), 0);

    ASSERT_EQ(0, tp.getColorValue(bitmap_image::color_plane::red_plane));
    ASSERT_EQ(0, tp.getColorValue(bitmap_image::color_plane::green_plane));
    ASSERT_EQ(0, tp.getColorValue(bitmap_image::color_plane::blue_plane));
}

UTEST(TranslatedPixel, singlePixelSingleColor) {
    TranslatedPixel tp;

    rgb_t color;
    color.red = 100;
    color.green = 0;
    color.blue = 0;

    int steps = 5;

    Pixel p(1, 1, color);
    std::vector<Pixel> pixels;
    pixels.push_back(p);

    tp = TranslatedPixel(std::move(pixels), steps);

    //value of 20 out of 255 possible values
    float fractionValue = (float)color.red/ 255;
    ASSERT_GT(fractionValue, 0);
    ASSERT_LT(fractionValue, 1);
    uint8_t strength = (int)round(fractionValue * steps);

    ASSERT_GT(strength, 0);
    ASSERT_LT(strength, steps);

    //Values are translated using the steps value
    ASSERT_EQ(strength, tp.getColorValue(bitmap_image::color_plane::red_plane));
    ASSERT_EQ(0, tp.getColorValue(bitmap_image::color_plane::green_plane));
    ASSERT_EQ(0, tp.getColorValue(bitmap_image::color_plane::blue_plane));
    ASSERT_TRUE(1);
}

UTEST(TranslatedPixel, singlePixelMultiColor) {
    TranslatedPixel tp;

    rgb_t color;
    color.red = 100;
    color.green = 50;
    color.blue = 200;

    int steps = 5;

    Pixel p(1, 1, color);
    std::vector<Pixel> pixels;
    pixels.push_back(p);

    tp = TranslatedPixel(std::move(pixels), steps);

    uint8_t redStrength = 0;
    uint8_t blueStrength = 0;
    uint8_t greenStrength = 0;
    {
        float fractionValue = (float)color.red/ 255;
        ASSERT_GT(fractionValue, 0);
        ASSERT_LT(fractionValue, 1);
        redStrength = (int)round(fractionValue * steps);
    }

    {
        float fractionValue = (float)color.green/ 255;
        ASSERT_GT(fractionValue, 0);
        ASSERT_LT(fractionValue, 1);
        greenStrength = (int)round(fractionValue * steps);
    }

    {
        float fractionValue = (float)color.blue/ 255;
        ASSERT_GT(fractionValue, 0);
        ASSERT_LT(fractionValue, 1);
        blueStrength = (int)round(fractionValue * steps);
    }

    ASSERT_GT(redStrength, 0);
    ASSERT_LT(redStrength, steps);
    ASSERT_GT(greenStrength, 0);
    ASSERT_LT(greenStrength, steps);
    ASSERT_GT(blueStrength, 0);
    ASSERT_LT(blueStrength, steps);

    ASSERT_EQ(2, redStrength);
    ASSERT_EQ(1, greenStrength);
    ASSERT_EQ(4, blueStrength);

    //Values are translated using the steps value
    ASSERT_EQ(redStrength, tp.getColorValue(bitmap_image::color_plane::red_plane));
    ASSERT_EQ(greenStrength, tp.getColorValue(bitmap_image::color_plane::green_plane));
    ASSERT_EQ(blueStrength, tp.getColorValue(bitmap_image::color_plane::blue_plane));
    ASSERT_TRUE(1);
}

UTEST(TranslatedPixel, twoPixelSingleColor) {
    TranslatedPixel tp;

    rgb_t colorOne;
    colorOne.red = 100;
    colorOne.green = 0;
    colorOne.blue = 0;

    rgb_t colorTwo;
    colorTwo.red = 200;
    colorTwo.green = 0;
    colorTwo.blue = 0;

    int steps = 5;

    Pixel p1(1, 1, colorOne);
    Pixel p2(1, 2, colorTwo);
    std::vector<Pixel> pixels;
    pixels.push_back(p1);
    pixels.push_back(p2);

    tp = TranslatedPixel(std::move(pixels), steps);

    //200+100 = 280; 300/2 = 150 - 5 steps = 3 strength
    float average = (float)(colorOne.red + colorTwo.red) / 2;

    float fractionValue = average/ 255;
    ASSERT_GT(fractionValue, 0);
    ASSERT_LT(fractionValue, 1);
    uint8_t strength = (int)round(fractionValue * steps);

    ASSERT_GT(strength, 0);
    ASSERT_LT(strength, steps);

    ASSERT_EQ(3, tp.getColorValue(bitmap_image::color_plane::red_plane));
    //Values are translated using the steps value
    ASSERT_EQ(strength, tp.getColorValue(bitmap_image::color_plane::red_plane));
    ASSERT_EQ(0, tp.getColorValue(bitmap_image::color_plane::green_plane));
    ASSERT_EQ(0, tp.getColorValue(bitmap_image::color_plane::green_plane));
    ASSERT_TRUE(1);
}