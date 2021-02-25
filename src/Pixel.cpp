//Copyright (c) 2021 Alex Gray

#include "Pixel.h"

Pixel::Pixel(const uint16_t x, const uint16_t y, const rgb_t color):
m_xPos(x),
m_yPos(y),
m_color(color)
{
}

uint16_t Pixel::getXPos() const
{
    return m_xPos; 
}

uint16_t Pixel::getYPos() const
{
    return m_yPos; 
}

rgb_t Pixel::getColor() const
{
    return m_color; 
}