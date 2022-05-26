//Copyright (c) 2021 Alex Gray
#pragma once
#include <bitmap_image.hpp>

class Utils
{
    public:
        static std::string colorToStringRGB(bitmap_image::color_plane color);
        static std::string colorToStringRGB(int color);

        static float getBufferOBJValue(const float tileSizeMM);

    private:
        //tile size, divided by this value will be added to each empty tile to 
        //increase the empty space to make up for printed lines overflowing
        static constexpr float LINE_OVERFLOW_BUFFER = 8;
};