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

    ASSERT_EQ(10, p.getX());
    ASSERT_EQ(15, p.getY());
    ASSERT_EQ(colour.red, p.getColorValue(bitmap_image::color_plane::red_plane));
}

UTEST(Pixel, colors) {
    rgb_t colour;
    colour.red = 20;
    colour.green = 30;
    colour.blue = 0;
    Pixel p(1, 1, colour);

    ASSERT_EQ(20, p.getColorValue(bitmap_image::color_plane::red_plane));
    ASSERT_EQ(30, p.getColorValue(bitmap_image::color_plane::green_plane));
    ASSERT_EQ(0, p.getColorValue(bitmap_image::color_plane::blue_plane));
}

UTEST(Pixel, lower_bounds) {
    rgb_t colour;
    colour.red = -1;

    Pixel p(1, 1, colour);

    ASSERT_EQ(255,p.getColorValue(bitmap_image::color_plane::red_plane));
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored  "-Woverflow"
UTEST(Pixel, upper_bounds) {
    rgb_t colour;
    colour.red = 256; // overflow to 0
    Pixel p(1, 1, colour);

    ASSERT_EQ(0, p.getColorValue(bitmap_image::color_plane::red_plane));
}
#pragma GCC diagnostic pop