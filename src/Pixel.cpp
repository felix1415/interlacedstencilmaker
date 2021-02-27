//Copyright (c) 2021 Alex Gray

#include "Pixel.h"
#include <bitmap_image.hpp>

Pixel::Pixel(const uint16_t x, const uint16_t y, const rgb_t color):
m_xPos(x),
m_yPos(y),
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

uint16_t Pixel::getX() const
{
    return m_xPos; 
}

uint16_t Pixel::getY() const
{
    return m_yPos; 
}