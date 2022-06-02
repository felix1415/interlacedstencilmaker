//Copyright (c) 2021 Alex Gray

#include "Position.h"
#include <sstream>
#include <iostream>

Position::Position()
{
}

Position::Position(const uint16_t x, const uint16_t y):
m_xPos(x),
m_yPos(y)
{
}

uint16_t Position::getX() const
{
    return m_xPos; 
}

uint16_t Position::getY() const
{
    return m_yPos; 
}

std::string Position::toString() const
{
    std::ostringstream ss;
    ss << getX() << ", " << getY();
    return ss.str();
}   

void Position::bump(const uint16_t value)
{
    m_xPos += value;
    m_yPos += value;
}
