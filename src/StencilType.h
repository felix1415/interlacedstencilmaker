//Copyright (c) 2022 Alex Gray

#pragma once
#include <cxxopts.hpp>

namespace StencilType
{
    enum Type {
        grayscale = 0, // two stencils, gray color
        rgb = 1, // eight stencils, Red, Green, Blue colors + grayscale stencil
        wrgb = 2, // six stencils, Red, Green, Blue colors, no grayscale stencil
        cmyk = 3, // eight stencils, Cyan, Magenta, Yellow, Key (Black)
        null = 4 // you shouldn't get here
    };

    static const Type All[] = { grayscale, rgb, wrgb, cmyk};

    Type getType(const cxxopts::ParseResult & pr);
}