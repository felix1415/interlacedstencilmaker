#include "utest.h"
#include "../src/Utils.h"

UTEST(Utils, colorToString) {
    ASSERT_STREQ("red", Utils::colorToString(bitmap_image::color_plane::red_plane));
    ASSERT_STREQ("green", Utils::colorToString(bitmap_image::color_plane::green_plane));
    ASSERT_STREQ("blue", Utils::colorToString(bitmap_image::color_plane::blue_plane));
}