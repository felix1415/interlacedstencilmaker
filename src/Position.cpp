//Copyright (c) 2021 Alex Gray

#include "Position.h"
#include <sstream>
#include <iostream>

Position::Position()
{
}

Position::Position(const float x, const float y):
m_xPos(x),
m_yPos(y)
{
}

float Position::getX() const
{
    return m_xPos; 
}

float Position::getY() const
{
    return m_yPos; 
}

std::string Position::toString() const
{
    std::ostringstream ss;
    ss << getX() << ", " << getY();
    return ss.str();
}   

void Position::bump(const float value)
{
    m_xPos += value;
    m_yPos += value;
}
