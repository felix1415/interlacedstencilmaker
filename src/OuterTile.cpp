//Copyright (c) 2021 Alex Gray

#include "OuterTile.h"
#include <sstream>

OuterTile::OuterTile(const uint16_t x, const uint16_t y, const float tileSizeMM):
Tile(x, y, tileSizeMM)
{
}


std::pair<std::vector<vertices>,std::vector<faces>> OuterTile::getOBJData(int & faceStartingNumber) const
{
    std::vector<vertices> verticesVec;
    std::vector<faces> facesVec;
    verticesVec.reserve(8); // 8 vertices
    facesVec.reserve(6); // 6 faces
    float xStart = 0.0f;
    float xEnd = m_tileSizeMM;
    float yStart = 0.0f;
    float yEnd = m_tileSizeMM; 


    //give the 0,0 tile a little notch so we know how to line up the final stencil
    if(getX() == 0 and getY() == 0)
    {
        yStart = m_tileSizeMM/2;
        xStart = m_tileSizeMM/2;
    }

    std::vector<vertices> otVerts = getVertices(xStart, xEnd, yStart, yEnd);
    verticesVec.insert(std::end(verticesVec), std::begin(otVerts), std::end(otVerts));

    std::vector<faces> otFaces = getFaces(faceStartingNumber);
    facesVec.insert(std::end(facesVec), std::begin(otFaces), std::end(otFaces));

    return std::make_pair(verticesVec, facesVec);
}

std::string OuterTile::toString() const
{
    std::ostringstream ss;
    ss << "OT - x:" << getX() << " y:" << getY();
    return ss.str();
}