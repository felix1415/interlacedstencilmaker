//Copyright (c) 2021 Alex Gray

#include <bitmap_image.hpp>

class Pixel
{
    public:
        Pixel(const uint16_t x, const uint16_t y, const rgb_t color);

        uint16_t getXPos() const;
        uint16_t getYPos() const; 
        rgb_t getColor() const;

    private:
        uint16_t m_xPos;
        uint16_t m_yPos;
        rgb_t m_color;
};