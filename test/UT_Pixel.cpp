#include "utest.h"
#include "../src/Pixel.h"

UTEST(Pixel, basic) {

    rgb_t colour;
    Pixel p(10, 15, colour);

    ASSERT_EQ(p.getX(), 10);
    ASSERT_EQ(p.getY(), 15);
}

UTEST(Pixel, basic2) {
    rgb_t colour;
    colour.red = 20;
    Pixel p(10, 15, colour);

    ASSERT_EQ(p.getX(), 10);
    ASSERT_EQ(p.getY(), 15);
    ASSERT_EQ(p.getColorValue(bitmap_image::color_plane::red_plane), colour.red);
}

UTEST(Pixel, colors) {
    rgb_t colour;
    colour.red = 20;
    colour.green = 30;
    colour.blue = 0;
    Pixel p(1, 1, colour);

    ASSERT_EQ(p.getColorValue(bitmap_image::color_plane::red_plane), 20);
    ASSERT_EQ(p.getColorValue(bitmap_image::color_plane::green_plane), 30);
    ASSERT_EQ(p.getColorValue(bitmap_image::color_plane::blue_plane), 0);
}

UTEST(Pixel, lower_bounds) {
    rgb_t colour;
    colour.red = -1;
    Pixel p(1, 1, colour);

    ASSERT_EQ(p.getColorValue(bitmap_image::color_plane::red_plane), 255);
}

UTEST(Pixel, upper_bounds) {
    rgb_t colour;
    colour.red = 256; // overflow to 0
    Pixel p(1, 1, colour);

    ASSERT_EQ(p.getColorValue(bitmap_image::color_plane::red_plane), 0);
}