//Copyright (c) 2021 Alex Gray

#include "App.h"
#include "Stencilator.h"
#include <ctime>
#include <iostream>
#include <sstream>
#include <cxxopts.hpp>

namespace cics
{
    Stencilator::Stencilator(const uint16_t width, const uint16_t height, const std::string &inputFile, const std::string & outputFile):
    m_width(width),
    m_height(height),
    m_inputFile(inputFile),
    m_outputFile(outputFile)
    {

    }

    int Stencilator::execute(const bool debug)
    {
        if(debug)
        {
            return 1;
        }
        return 0;
    }

}