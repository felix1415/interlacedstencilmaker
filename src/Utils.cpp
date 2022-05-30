//Copyright (c) 2021 Alex Gray

#include "Utils.h"

std::string Utils::colorToStringRGB(const int color)
{
    return colorToStringRGB((bitmap_image::color_plane)color);
}

std::string Utils::colorToStringRGB(const bitmap_image::color_plane color)
{
    switch(color)
    {
        case 0:
            return "red";
        case 1:
            return "green";
        case 2:
            return "blue";
        default:
            return "NotAColor";
    }
}

std::string Utils::colorToStringCMYK(const int color)
{
    switch(color)
    {
        case 0:
            return "cyan";
        case 1:
            return "magenta";
        case 2:
            return "yellow";
        case 3:
            return "key";
        default:
            return "NotAColor";
    }
}

float Utils::getBufferOBJValue(const float tileSizeMM)
{
    return (tileSizeMM / LINE_OVERFLOW_BUFFER);
}