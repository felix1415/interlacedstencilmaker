//Copyright (c) 2021 Alex Gray

#include "Utils.h"

std::string Utils::colorToString(int color)
{
    return colorToString((bitmap_image::color_plane)color);
}

std::string Utils::colorToString(bitmap_image::color_plane color)
{
    switch(color)
    {
        case bitmap_image::color_plane::red_plane:
            return "red";
        case bitmap_image::color_plane::green_plane:
            return "green";
        case bitmap_image::color_plane::blue_plane:
            return "blue";
        default:
            return "NotAColor";
    }
}

float Utils::getBufferOBJValue(const float tileSizeMM)
{
    return (tileSizeMM / LINE_OVERFLOW_BUFFER);
}