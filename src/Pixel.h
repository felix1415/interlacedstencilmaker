//Copyright (c) 2021 Alex Gray

#pragma once
#include "Position.h"
#include <bitmap_image.hpp>
#include <map>

class Pixel : public Position
{
    public:
        typedef std::array<int, 3> Colors;

    public:
        Pixel(const uint16_t x, const uint16_t y, const rgb_t color);
        Pixel();

        int getColorValue(const bitmap_image::color_plane color) const;
        const Colors & getColorArray() const;

    protected:
        const rgb_t & getColor() const;
        
    private:
        void calculateTranslatedColourValue();

    private:
        Colors m_colorArray;
        rgb_t m_color;
};