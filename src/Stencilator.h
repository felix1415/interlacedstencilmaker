//Copyright (c) 2021 Alex Gray

#include <iostream>
#include <string>

namespace cics
{
    class Stencilator
    {
        public:
            Stencilator(const uint16_t width, const uint16_t height, const std::string &inputFile, const std::string & outputFile);
            int execute(const bool debug);

        private:
            uint16_t m_width;
            uint16_t m_height;
            std::string m_inputFile;
            std::string m_outputFile;
    };
}