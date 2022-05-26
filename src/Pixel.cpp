//Copyright (c) 2021 Alex Gray

#include "Pixel.h"
#include "Utils.h"
#include <bitmap_image.hpp>
#include <sstream>

Pixel::Pixel()
{
}

Pixel::Pixel(const Position & position):
Position(position)
{
}

Pixel::Pixel(const uint16_t x, const uint16_t y, const rgb_t color):
Position(x, y),
m_color(color)
{
    m_colorArray[bitmap_image::color_plane::red_plane] = (int)m_color.red;
    m_colorArray[bitmap_image::color_plane::green_plane] = (int)m_color.green;
    m_colorArray[bitmap_image::color_plane::blue_plane] = (int)m_color.blue;
}

int Pixel::getColorValue(const bitmap_image::color_plane color) const
{
    return m_colorArray[color];
}

const rgb_t & Pixel::getColor() const
{
    return m_color;
}

const Pixel::Colors & Pixel::getColorArray() const
{
    return m_colorArray;
}

std::string Pixel::toString() const
{
    std::ostringstream ss;
    ss << Position::toString() << "\n";
    for(size_t i = 0; i < getColorArray().size(); i++)
    {
        ss << "    " << Utils::colorToString(i) << ":"<< m_colorArray[i] << " ";
    }
    ss  << "\n";

    return ss.str();
}   
