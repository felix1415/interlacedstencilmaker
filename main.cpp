#include <iostream>
#include "src/SingleTile.h"
#include "src/App.h"

int main(int argc, char *argv[])
{
    try
    {
        return cics::App().run(argc, argv);
    }
    catch(...)
    {
        std::cerr << "App failed to run" << std::endl;
        return EXIT_FAILURE;
    }
}