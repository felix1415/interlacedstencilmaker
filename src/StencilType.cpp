//Copyright (c) 2022 Alex Gray
#include "StencilType.h"

namespace StencilType
{
    Type getType(const cxxopts::ParseResult & pr)
    {
        if(pr.count("grayscale"))
        {
            return StencilType::Type::grayscale;
        }

        if(pr.count("rgb"))
        {
            return StencilType::Type::rgb;
        }

        if(pr.count("wrgb"))
        {
            return StencilType::Type::wrgb;
        }

        if(pr.count("cmyk"))
        {
            return StencilType::Type::cmyk;
        }

        throw std::runtime_error("tried to get stencil type, but wasn't given one");
    }
}