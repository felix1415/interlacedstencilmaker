//Copyright (c) 2021 Alex Gray
#pragma once
#include <stdint.h>
#include <string>

class Position
{
    public:
        Position();
        Position(const float x, const float y);

        float getX() const;
        float getY() const;
        std::string toString() const;
        void bump(const float value);

    private:
        float m_xPos;
        float m_yPos;
};