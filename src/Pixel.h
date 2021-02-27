//Copyright (c) 2021 Alex Gray

#pragma once
#include <bitmap_image.hpp>
#include <map>

class Pixel
{
    public:
        typedef std::array<int, 3> Colors;

    public:
        Pixel(const uint16_t x, const uint16_t y, const rgb_t color);
        Pixel();

        uint16_t getX() const;
        uint16_t getY() const; 
        int getColorValue(const bitmap_image::color_plane color) const;

    protected:
        const rgb_t & getColor() const;
        const Colors & getColorArray() const;

    private:
        void calculateTranslatedColourValue();

    private:
        uint16_t m_xPos;
        uint16_t m_yPos;
        Colors m_colorArray;
        rgb_t m_color;
};