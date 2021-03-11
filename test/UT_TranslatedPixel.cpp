#include "utest.h"
#include "../src/Position.h"
#include "../src/Pixel.h"
#include "../src/TranslatedPixel.h"
#include <memory>

UTEST(TranslatedPixel, basic) {
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

    ASSERT_EQ(tp->getX(), 10);
    ASSERT_EQ(tp->getY(), 15);

    //Values are translated using the steps value
    ASSERT_NE(20, (int)tp->getTranslatedColorValue(bitmap_image::color_plane::red_plane));
    ASSERT_NE(30, (int)tp->getTranslatedColorValue(bitmap_image::color_plane::green_plane));
    ASSERT_NE(40, (int)tp->getTranslatedColorValue(bitmap_image::color_plane::blue_plane));
}

UTEST(TranslatedPixel, basicEmpty) {
    std::unique_ptr<TranslatedPixel> tp;
    Position pos(0,0);

    rgb_t color;
    color.red = 0;
    color.green = 0;
    color.blue = 0;

    Pixel p(0, 0, color);
    std::vector<Pixel> pixels;
    pixels.push_back(p);

    int steps = 0;
    float tileSizeMM = 0.0f;

    tp.reset(new TranslatedPixel(pos, std::move(pixels), steps, tileSizeMM));

    ASSERT_EQ(0, tp->getX());
    ASSERT_EQ(0, tp->getY());

    ASSERT_EQ(0.0f, tp->getTranslatedColorValue(bitmap_image::color_plane::red_plane));
    ASSERT_EQ(0.0f, tp->getTranslatedColorValue(bitmap_image::color_plane::green_plane));
    ASSERT_EQ(0.0f, tp->getTranslatedColorValue(bitmap_image::color_plane::blue_plane));
}

UTEST(TranslatedPixel, singlePixelSingleColor) {
    std::unique_ptr<TranslatedPixel> tp;
    Position pos(10,15);

    rgb_t color;
    color.red = 100;
    color.green = 0;
    color.blue = 0;

    int steps = 5;
    float tileSizeMM = 1.0f;

    Pixel p(1, 1, color);
    std::vector<Pixel> pixels;
    pixels.push_back(p);

    tp.reset(new TranslatedPixel(pos, std::move(pixels), steps, tileSizeMM));

    //value of 20 out of 255 possible values
    float fractionValue = (float)color.red/ 255;
    ASSERT_GT(fractionValue, 0);
    ASSERT_LT(fractionValue, 1);
    uint8_t stepValue = (int)round(fractionValue * steps);

    float calculatedStrengthValue = (tileSizeMM/steps) * stepValue;

    ASSERT_GT(stepValue, 0);
    ASSERT_LT(stepValue, steps);

    //Values are translated using the steps value
    ASSERT_EQ(calculatedStrengthValue, tp->getTranslatedColorValue(bitmap_image::color_plane::red_plane));
    ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::green_plane));
    ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::blue_plane));
}

UTEST(TranslatedPixel, singlePixelMultiColor) {
    std::unique_ptr<TranslatedPixel> tp;
    Position pos(10,15);

    rgb_t color;
    color.red = 100;
    color.green = 50;
    color.blue = 200;

    int steps = 5;
    float tileSizeMM = 1.0f;

    Pixel p(1, 1, color);
    std::vector<Pixel> pixels;
    pixels.push_back(p);

    tp.reset(new TranslatedPixel(pos, std::move(pixels), steps, tileSizeMM));

    uint8_t redStepValue = 0;
    float redCalculatedStrengthValue = 0;
    uint8_t greenStepValue = 0;
    float greenCalculatedStrengthValue = 0;
    uint8_t blueStepValue = 0;
    float blueCalculatedStrengthValue = 0;

    {
        float fractionValue = (float)color.red/ 255;
        ASSERT_GT(fractionValue, 0);
        ASSERT_LT(fractionValue, 1);
        redStepValue = (int)round(fractionValue * steps);
        redCalculatedStrengthValue = (tileSizeMM/steps) * redStepValue;
    }

    {
        float fractionValue = (float)color.green/ 255;
        ASSERT_GT(fractionValue, 0);
        ASSERT_LT(fractionValue, 1);
        greenStepValue = (int)round(fractionValue * steps);
        greenCalculatedStrengthValue = (tileSizeMM/steps) * greenStepValue;
    }

    {
        float fractionValue = (float)color.blue/ 255;
        ASSERT_GT(fractionValue, 0);
        ASSERT_LT(fractionValue, 1);
        blueStepValue = (int)round(fractionValue * steps);
        blueCalculatedStrengthValue = (tileSizeMM/steps) * blueStepValue;
    }

    ASSERT_GT(redStepValue, 0);
    ASSERT_LT(redStepValue, steps);
    ASSERT_GT(greenStepValue, 0);
    ASSERT_LT(greenStepValue, steps);
    ASSERT_GT(blueStepValue, 0);
    ASSERT_LT(blueStepValue, steps);

    ASSERT_EQ(0.4f, redCalculatedStrengthValue);
    ASSERT_EQ(0.2f, greenCalculatedStrengthValue);
    ASSERT_EQ(0.8f, blueCalculatedStrengthValue);

    //Values are translated using the steps value
    ASSERT_EQ(redCalculatedStrengthValue, tp->getTranslatedColorValue(bitmap_image::color_plane::red_plane));
    ASSERT_EQ(greenCalculatedStrengthValue, tp->getTranslatedColorValue(bitmap_image::color_plane::green_plane));
    ASSERT_EQ(blueCalculatedStrengthValue, tp->getTranslatedColorValue(bitmap_image::color_plane::blue_plane));
}

UTEST(TranslatedPixel, twoPixelSingleColor) {
    std::unique_ptr<TranslatedPixel> tp;
    Position pos(10,15);

    rgb_t colorOne;
    colorOne.red = 100;
    colorOne.green = 0;
    colorOne.blue = 0;

    rgb_t colorTwo;
    colorTwo.red = 200;
    colorTwo.green = 0;
    colorTwo.blue = 0;

    int steps = 5;
    float tileSizeMM = 1.0f;

    Pixel p1(1, 1, colorOne);
    Pixel p2(1, 2, colorTwo);
    std::vector<Pixel> pixels;
    pixels.push_back(p1);
    pixels.push_back(p2);

    tp.reset(new TranslatedPixel(pos, std::move(pixels), steps, tileSizeMM));

    //200+100 = 280; 300/2 = 150 - 5 steps = 3 strength
    float average = (float)(colorOne.red + colorTwo.red) / 2;

    float fractionValue = average/ 255;
    ASSERT_GT(fractionValue, 0);
    ASSERT_LT(fractionValue, 1);
    uint8_t stepValue = (int)round(fractionValue * steps);
    float calculatedStrengthValue = (tileSizeMM/steps) * stepValue;

    ASSERT_GT(stepValue, 0);
    ASSERT_LT(stepValue, steps);


    //Values are translated using the steps value
    ASSERT_EQ(calculatedStrengthValue, tp->getTranslatedColorValue(bitmap_image::color_plane::red_plane));
    ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::green_plane));
    ASSERT_EQ(0, tp->getTranslatedColorValue(bitmap_image::color_plane::blue_plane));
}