//Copyright (c) 2021 Alex Gray

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