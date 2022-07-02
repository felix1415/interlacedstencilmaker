//Copyright (c) 2021 Alex Gray

#pragma once

#include <string>
#include <vector>

  
struct vertices
{
  float   p1;
  float   p2;
  float   p3;
};

struct faces
{
  uint32_t   f1;
  uint32_t   f2;
  uint32_t   f3;
  uint32_t   f4;
};

struct TriangleStats
{
    float a;
    float b;
    float c;
    float area;
    float angle;
};

static const float STENCIL_THICKNESS = 0.6f;

typedef std::pair<std::vector<vertices>,std::vector<faces>> OBJData;
OBJData & operator+=(OBJData & a, const OBJData & b);

class OBJ
{
    public:
        static void writeOBJFile(const std::string outputFile, const std::vector<vertices> & vertices, const std::vector<faces> & faces);

        //x : x location of Right angle C
        //y : y location of Right angle C
        //a : vertical to angle B, offset from x (length a)
        //a : horizontal to angle A, offset from x (length b)
        // https://upload.wikimedia.org/wikipedia/commons/thumb/6/6f/Rtriangle.svg/330px-Rtriangle.svg.png
        static std::vector<vertices> getTriangleVertices(float x, float y, float a, float b);
};