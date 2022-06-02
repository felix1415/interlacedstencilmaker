#include "utest.h"
#include "../src/Utils.h"

UTEST(Utils, colorToStringRGB) {
    ASSERT_STREQ("red", Utils::colorToStringRGB(bitmap_image::color_plane::red_plane).c_str());
    ASSERT_STREQ("green", Utils::colorToStringRGB(bitmap_image::color_plane::green_plane).c_str());
    ASSERT_STREQ("blue", Utils::colorToStringRGB(bitmap_image::color_plane::blue_plane).c_str());
}

UTEST(Utils, colorToStringCMYK) {
    ASSERT_STREQ("cyan", Utils::colorToStringCMYK(0).c_str());
    ASSERT_STREQ("magenta", Utils::colorToStringCMYK(1).c_str());
    ASSERT_STREQ("yellow", Utils::colorToStringCMYK(2).c_str());
    ASSERT_STREQ("key", Utils::colorToStringCMYK(3).c_str());
}