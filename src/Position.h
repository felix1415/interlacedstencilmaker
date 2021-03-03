//Copyright (c) 2021 Alex Gray
#pragma once
#include <stdint.h>
#include <string>

class Position
{
    public:
        Position();
        Position(const uint16_t x, const uint16_t y);

        uint16_t getX() const;
        uint16_t getY() const; 
        std::string toString() const;

    private:
        uint16_t m_xPos;
        uint16_t m_yPos;
};