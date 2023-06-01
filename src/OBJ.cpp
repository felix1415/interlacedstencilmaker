
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "OBJ.h"


OBJData & operator+=(OBJData & a, const OBJData & b)
{
    a.first.insert(a.first.end(), b.first.begin(), b.first.end() );
    a.second.insert(a.second.end(), b.second.begin(), b.second.end() );
    return a;
}


void OBJ::writeOBJFile(const std::string outputFile, const std::vector<vertices> & vertices, const std::vector<faces> & faces)
{
    std::ofstream objFile;

    std::remove(outputFile.c_str());

    try
    {
        objFile.open (outputFile);
        objFile << "# stencil export from https://github.com/felix1415/interlacedstencilmaker";
        objFile << "\n";
        objFile << "mtllib obj.mtl\n";
        objFile << "\n";
        objFile << "o obj_0\n";

        for(const auto & vert : vertices)
        {
            objFile << "v " << vert.p1 << "    " << vert.p2 << "    " << vert.p3 << "\n";
        }
        objFile << "# " << vertices.size() << " vertices\n";

        objFile << "\n";
        objFile << "g group_0_15277357\n";
        objFile << "\n";
        objFile << "usemtl color_15277357\n";
        objFile << "s 0\n";
        objFile << "\n";

        for(const auto & face : faces)
        {
            objFile << "f " << face.f1 << "    " << face.f2 << "    " << face.f3;
            if(not face.f4 == 0)
                objFile << "    " << face.f4 << "\n";
            else //in the case of triangle, there's no 4th face
                objFile << "\n";
        }
        objFile << "# " << faces.size() << " faces\n";

        objFile << "# end of obj_0\n";
        objFile.close();

        std::cout << "Wrote " << outputFile << std::endl;
    }
    catch(...)
    {
        std::cout << "Error while writing OBJ file " << outputFile << std::endl;
    }
}

std::vector<vertices> OBJ::getTriangleVertices(float x, float y, float a, float b)
{
    std::vector<vertices> verticesToReturn = {
        {x,         y,          0},
        {x + b,     y,          0},
        {x,         y + a,      0},
        {x + b,     y,          STENCIL_THICKNESS},
        {x,         y,          STENCIL_THICKNESS},
        {x,         y + a,      STENCIL_THICKNESS},
    };

    return verticesToReturn;

    // // v 0        -20         0
    // // v 0        -20         20
    // // v 0        0       0
    // // v 20        -20         0
    // // v 20        -20         20
    // // v 20        0       0

    // //up by 5
    // // v 10        -20         5
    // // v 10        -20         25
    // // v 10        0       5            
    // // v 30        -20         5
    // // v 30        -20         25
    // // v 30        0       5

    // //left by 5
    // // v 10        -15         0
    // // v 10        -15         20
    // // v 10        5       0
    // // v 30        -15         20
    // // v 30        -15         0
    // // v 30        5       0

    // v 4         -20         0
    // v 4         -20         20
    // v 4         0       0
    // v 24        -20         20
    // v 24        -20         0
    // v 24        0       0
}