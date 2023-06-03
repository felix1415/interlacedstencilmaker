//Copyright (c) 2021 Alex Gray
#pragma once
#include <bitmap_image.hpp>

class Utils
{
    public:
        static std::string colorPlaneToStringRGB(const bitmap_image::color_plane color);
        static std::string colorToStringRGB(const int color);
        static std::string colorToStringWRGB(const int color);
        static std::string colorToStringGrayscale(const int color);
        static std::string colorToStringCMYK(const int color);

        static float getBufferOBJValue(const float tileSizeMM);

        static float floatCompare(const float a, const float b);

    private:
        //tile size, divided by this value will be added to each empty tile to 
        //increase the empty space to make up for printed lines overflowing
        static constexpr float LINE_OVERFLOW_BUFFER = 8;
        static constexpr float EPSILON = 0.00001f;
};