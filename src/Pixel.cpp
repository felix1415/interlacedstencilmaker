//Copyright (c) 2021 Alex Gray

#include "Pixel.h"
#include "Utils.h"
#include <bitmap_image.hpp>
#include <sstream>
#include <math.h> 

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
    m_colorArray[0] = (int)m_color.red;
    m_colorArray[1] = (int)m_color.green;
    m_colorArray[2] = (int)m_color.blue;
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

Pixel & Pixel::operator+=(const Pixel& a)
{
    m_pixelsRepresented++;
    for(size_t i = 0; i < getColorArray().size(); i++)
    {
        float oldAvg = this->m_colorArray[i];
        float newValue = a.getColorArray()[i];
        float newAvg = oldAvg + ((newValue - oldAvg) / m_pixelsRepresented);


        m_acc[i] += a.getColorArray()[i];


        this->m_colorArray[i] = std::round(newAvg);
    }

    return *this;
}

std::string Pixel::toString() const
{
    std::ostringstream ss;
    ss << Position::toString() << "\n";
    for(size_t i = 0; i < getColorArray().size(); i++)
    {
        ss << "    " << Utils::colorToStringRGB(i) << ":# "<< m_colorArray[i] << " ";
        ss  << "\n";
        float test = m_acc[i] / m_pixelsRepresented;
        ss << "t " << test;
        ss  << "\n";
    }


    return ss.str();
}   
