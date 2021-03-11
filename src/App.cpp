//Copyright (c) 2021 Alex Gray

#include "App.h"
#include "Stencilator.h"
#include "Utils.h"
#include <cxxopts.hpp>
#include <ctime>
#include <iostream>
#include <sstream>

namespace cics
{
    int App::run(int argc, char * argv[])
    {
        std::cout << "3 Colour Interlaced Stencil Creator" << std::endl;

        cxxopts::Options options("Interlaced Stencil Maker", "A small utility for turning bitmap images into Interlaced rgb stencils");

        try
        {
            options.add_options()
              ("d,debug", "Debugging mode", cxxopts::value<bool>()->default_value("false"))
              ("s,struts", "Model generated with struts for support", cxxopts::value<bool>()->default_value("false"))
              ("w,width", "Width of image", cxxopts::value<uint16_t>())
              ("h,height", "Height of image", cxxopts::value<uint16_t>())
              ("i,input", "Input file", cxxopts::value<std::string>())
              ("o,output", "Output folder", cxxopts::value<std::string>()->default_value("ISM_Output"))
              ;
        }
        catch(const cxxopts::OptionSpecException & e)
        {
            std::cout << "ERROR: " << e.what() << std::endl;
        }

        auto result = options.parse(argc, argv);

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

        return Stencilator(result["width"].as<uint16_t>(), 
                           result["height"].as<uint16_t>(), 
                           result["input"].as<std::string>(),
                           result["output"].as<std::string>(),
                           result["debug"].as<bool>(),
                           result["struts"].as<bool>()
                           ).execute();
    }
}