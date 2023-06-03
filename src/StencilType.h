//Copyright (c) 2022 Alex Gray

#pragma once
#include <cxxopts.hpp>

namespace StencilType
{
    enum Type {
        grayscale = 0, // two stencils, gray color
        rgb = 1, // six stencils, Red, Green, Blue colors stencil
        cmyk = 3, // eight stencils, Cyan, Magenta, Yellow, Key (Black)
        null = 4 // you shouldn't get here
    };

    static const Type All[] = { grayscale, rgb, cmyk};

    Type getType(const cxxopts::ParseResult & pr);
    int numberOfColors(const Type type);
    std::function<std::string(int)>  colorNameFunction(const Type type);
}