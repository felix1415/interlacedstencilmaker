//Copyright (c) 2021 Alex Gray
#pragma once
#include <string>

namespace cics
{
    class App
    {
        public:
            int run(int argc, char * argv[]);

        private:
            std::string getDefaultOutputFolder();
    };
}