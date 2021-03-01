//Copyright (c) 2021 Alex Gray

#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
  
struct vertices
{
  float   p1;
  float   p2;
  float   p3;
};

struct faces
{
  uint16_t   f1;
  uint16_t   f2;
  uint16_t   f3;
  uint16_t   f4;
};

typedef std::pair<std::vector<vertices>,std::vector<faces>> OBJData;


class OBJ
{
    public:
        static void writeOBJFile(const std::string outputFile, const std::vector<vertices> vertices, const std::vector<faces> faces)
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
                    objFile << "v " << vert.p1 *100 << "    " << vert.p2*100 << "    " << vert.p3*100 << "\n";
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
                    objFile << "f " << face.f1 << "    " << face.f2 << "    " << face.f3 << "    " << face.f4 << "\n";
                }
                objFile << "# " << faces.size() << " faces\n";

                objFile << "# end of obj_0\n";
                objFile.close();
            }
            catch(...)
            {
                std::cout << "Error while writing OBJ file " << outputFile << std::endl;
            }
        }

};