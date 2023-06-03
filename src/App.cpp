//Copyright (c) 2021 Alex Gray

#include "App.h"
#include "Stencilator.h"
#include "StencilType.h"
#include "Utils.h"
#include <cxxopts.hpp>
#include <ctime>
#include <iostream>
#include <sstream>

int App::run(int argc, char * argv[])
{
    std::cout << "3 Colour Interlaced Stencil Creator" << std::endl;

    cxxopts::Options options("Interlaced Stencil Maker", "A small utility for turning bitmap images into Interlaced stencils");

    try
    {
        options.add_options()
          ("D,debug", "Debugging mode", cxxopts::value<bool>()->default_value("false"))
          ("I,info", "Info mode", cxxopts::value<bool>()->default_value("false"))
          ("g,grayscale", "Generate grayscale stencil instead of an RGB collection of stencils", cxxopts::value<bool>()->default_value("false"))
          ("r,rgb", "Generate an RGB set of stencils and grayscale", cxxopts::value<bool>()->default_value("false"))
          ("w,wrgb", "Generate an RGB set of stencils to complement the grayscale", cxxopts::value<bool>()->default_value("false"))
          ("c,cmyk", "Generate an CMYK set of stencils", cxxopts::value<bool>()->default_value("false"))
          ("W,width", "Width of image", cxxopts::value<uint16_t>())
          ("H,height", "Height of image", cxxopts::value<uint16_t>())
          ("i,input", "Input file", cxxopts::value<std::string>())
          ("o,output", "Output folder", cxxopts::value<std::string>()->default_value("ISM"))
          ("h,help", "This help message", cxxopts::value<bool>()->default_value("false"))
          ;
    }
    catch(const cxxopts::OptionSpecException & e)
    {
        std::cout << options.help() << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
    }

    try
    {
        auto result = options.parse(argc, argv);

        if (result.count("help"))
        {
            std::cout << options.help() << std::endl;
            exit(0);
        }

        if(not result.count("input"))
        {
            std::cerr << "Input file not defined" << std::endl;
            return 0;
        }

        if(not result.count("width"))
        {
            std::cerr << "Width of picture not defined" << std::endl;
            return 0;
        }

        if(not result.count("height"))
        {
            std::cerr << "Height of picture not defined" << std::endl;
            return 0;
        }

        StencilType::Type stencilType = StencilType::getType(result);

        return Stencilator(result["width"].as<uint16_t>(), 
                   result["height"].as<uint16_t>(), 
                   result["input"].as<std::string>(),
                   result["output"].as<std::string>(),
                   result["info"].as<bool>(),
                   result["debug"].as<bool>(),
                   stencilType
                   ).execute();
    }
    catch(const cxxopts::OptionParseException & e)
    {
        std::cout << options.help() << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
    }

    return 1;
}
