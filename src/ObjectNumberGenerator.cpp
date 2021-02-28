//Copyright (c) 2021 Alex Gray

#include "ObjectNumberGenerator.h"

// ObjectNumberGenerator::ObjectNumberGenerator()
// {
// }

int ObjectNumberGenerator::nextValue()
{
    return ++m_value;
}