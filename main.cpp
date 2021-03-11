//Copyright (c) 2021 Alex Gray

#include <iostream>
#include "src/App.h"

int main(int argc, char *argv[])
{
    try
    {
        return App().run(argc, argv);
    }
    catch(...)
    {
        std::cerr << "App failed to run" << std::endl;
        return EXIT_FAILURE;
    }
}