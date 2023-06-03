//Copyright (c) 2021 Alex Gray

#include "Utils.h"

std::string Utils::colorPlaneToStringRGB(const bitmap_image::color_plane color)
{
    switch(color)
    {
        case bitmap_image::color_plane::blue_plane:
            return colorToStringRGB(2);
        case bitmap_image::color_plane::green_plane:
            return colorToStringRGB(1);
        case bitmap_image::color_plane::red_plane:
            return colorToStringRGB(0);
        default:
            return "NotAColor";
    }
}

std::string Utils::colorToStringRGB(const int color)
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

std::string Utils::colorToStringGrayscale(const int color)
{
    if (color != 1)
    {
        throw std::runtime_error("a color other than 1 was passed to colorToStringGrayscale");
    }
    return "grayscale";
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

float Utils::floatCompare(const float a, const float b)
{
    float difference = std::abs(a - b);
    return difference < EPSILON;
}