//Copyright (c) 2021 Alex Gray

#include "Position.h"

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