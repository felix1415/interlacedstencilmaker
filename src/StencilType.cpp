//Copyright (c) 2022 Alex Gray
#include "StencilType.h"
#include "Utils.h"

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

    int numberOfColors(const Type type)
    {
        switch(type)
        {
            case Type::grayscale:
                return 1;
            case Type::rgb:
                return 4;
            case Type::cmyk:
                return 4;

            default:
                return 0;
        }
    }

    std::function<std::string(int)> colorNameFunction(const Type type)
    {
        switch(type)
        {
            case Type::grayscale:
                return Utils::colorToStringGrayscale;
            case Type::rgb:
                return Utils::colorToStringRGB;
            case Type::cmyk:
                return Utils::colorToStringCMYK;

            default:
                throw std::runtime_error("no type passed to colorNameFunction");
        }
    }
}