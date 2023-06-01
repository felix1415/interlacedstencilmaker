//Copyright (c) 2021 Alex Gray

#include "CMYKTile.h"
#include "Utils.h"
#include <bitmap_image.hpp>
#include <math.h>
#include <iostream>
#include <sstream>

CMYKTile::CMYKTile(std::unique_ptr<TranslatedPixel> && pixel):
Tile(std::move(pixel)),
m_bufferVal(Utils::getBufferOBJValue(m_tileSizeMM))
{
}

OBJData CMYKTile::generateOBJData(int & faceStartingNumber, const float xStart,  const float xEnd,  const float yStart,  const float yEnd) const
{
    std::vector<vertices> verticesVec;
    std::vector<faces> facesVec;
    verticesVec.reserve(INNER_TILE_VERTS);
    facesVec.reserve(INNER_TILE_FACES);
    TileOptions tOptions = TileOptions::none;

    std::vector<vertices> colorVerts = getVertices(xStart, xEnd, yStart, yEnd);
    verticesVec.insert(std::end(verticesVec), std::begin(colorVerts), std::end(colorVerts));
        
    std::vector<faces> colorFaces = getFaces(faceStartingNumber, tOptions);
    facesVec.insert(std::end(facesVec), std::begin(colorFaces), std::end(colorFaces));

    return std::make_pair(verticesVec, facesVec);
}

void CMYKTile::getStatsOfTriangle(TriangleStats & stats) const
{
    stats.c = stats.b * (1 / cos(stats.angle*(M_PI/180)));
    stats.a = sqrt(stats.c*stats.c - stats.b*stats.b);
    stats.area = (stats.a * stats.b) / 2;
}


//write a test to put this into an OBJ file to inspect it's actuially working properly
OBJData CMYKTile::addRTriangle(int & faceStartingNumber, const float b, const uint16_t angle, const float yValue, const bool flip, const int aDirection) const
{
    float c = b * (1 / cos(angle*(M_PI/180)));
    float a = sqrt(c*c - b*b) * aDirection;

    //if we're changing the direction of the triangle in the y axis, we need 
    //to shift it by a tile on the x axis to flip it upside down
    const float x = (getX() * m_tileSizeMM) + ((aDirection == -1) ? m_tileSizeMM : 0);
    //add the position of the triangle in the tile, x,y are from 0-BF,0-BF of the tile BF=bufferValue
    const float y = (getY() * m_tileSizeMM) + yValue;

    //flip will change the direction horizontally
    OBJData objd;
    if(flip)
        objd.first = OBJ::getTriangleVertices(x + b, y, a, -b);
    else
        objd.first = OBJ::getTriangleVertices(x, y, a, b);

    objd.second = getTriangleFaces(faceStartingNumber);

    return objd;
}

OBJData CMYKTile::addRectangle(int & faceStartingNumber, const float area, const float y) const
{
    float yHeight = area / m_tileSizeMM;
    OBJData objd = addBlock(0, m_tileSizeMM, y, y + yHeight, faceStartingNumber);

    return objd;
}

OBJData CMYKTile::addBlockingShapes(int & faceStartingNumber, const uint16_t angle, const bool flip, const float areaToOpen) const
{
    //blocking shapes angle a pixel on rotation of the pixel, we want to workout 
    //the area we want open will be the gap for the pixel, we there for need to 
    //blocks at bottom of the pixel and then triangles on top to create
    //angled opening

    TriangleStats triangleStats;
    triangleStats.angle = angle;
    triangleStats.b = m_tileSizeMM;

    //how much space in total we have to work with
    float areaBlockable = m_tileSizeMM * (m_tileSizeMM + (m_bufferVal * 2));
    //the area both triangles will take up. One above, one below
    getStatsOfTriangle(triangleStats); 
    //the area if both the open space and the triangles are fulfilled, we need 
    //to block up futher, this is optional and won't be placed if it's not needed, 
    //i.e. fully open tile
    float additionalBlockAreaRequired = areaBlockable - ((triangleStats.area * 2) + areaToOpen);

    // we start at the bottom of the tile, but these tiles are opened in the Y 
    //axis by a buffer value, so we start from y lower than buffer value
    float yValue = -m_bufferVal;
    OBJData objDataAcc; // accumalate the obj data as we add more shapes

    //order of shapes:
    //block
    //triangle
    //space
    //triangle

    //optional additional quad block
    if(additionalBlockAreaRequired > 0.0f)
    {
        objDataAcc += addRectangle(faceStartingNumber, additionalBlockAreaRequired, yValue);
        yValue += additionalBlockAreaRequired / m_tileSizeMM;
    }

    //add the bottom triangle
    objDataAcc += addRTriangle(faceStartingNumber, m_tileSizeMM, angle, yValue, flip, 1);
    yValue += triangleStats.c;
    
    float yGap = (areaToOpen / m_tileSizeMM);
    yValue += yGap;

    yValue = m_tileSizeMM + m_bufferVal;

    //add the upper triangle, but upside down
    objDataAcc += addRTriangle(faceStartingNumber, -m_tileSizeMM, angle, yValue, flip, -1);

    return objDataAcc;
}


OBJData CMYKTile::getOBJData(int & faceStartingNumber, const int color, const bool plate) const
{
    float xStart = 0.0f;
    float xEnd = 0.0f;
    float yStart = 0.0f; 
    float yEnd = 0.0f;

    //optimisation for when no color is required
    if(m_translatedPixel->getTranslatedColorValue(color) < 0.001f)
    {
        xStart = 0.0f;
        xEnd = m_tileSizeMM;
        yStart =  0.0f - m_bufferVal; 
        yEnd = m_tileSizeMM + m_bufferVal;
        return generateOBJData(faceStartingNumber, xStart, xEnd, yStart, yEnd);
    }

    switch(color)
    {
        case 0:
            //bottom x, full y
            //blocks bottom upwards
            
            xStart = 0.0f;
            xEnd = m_tileSizeMM;
            yStart = 0.0f - m_bufferVal; 
            yEnd = (m_tileSizeMM) - m_translatedPixel->getTranslatedColorValue(color);

            if(plate)
            {
                return addBlockingShapes(faceStartingNumber, 20, false, (m_tileSizeMM * m_translatedPixel->getTranslatedColorValue(color)) + m_bufferVal);
            }

            break;
        case 1:

            xStart = 0.0f;
            xEnd = m_tileSizeMM;
            yStart = 0.0f - m_bufferVal; 
            yEnd = (m_tileSizeMM) - m_translatedPixel->getTranslatedColorValue(color);

            if(not plate)
            {
                return addBlockingShapes(faceStartingNumber, 20, false, (m_tileSizeMM * m_translatedPixel->getTranslatedColorValue(color)) + m_bufferVal);
            }

            break;
        case 2: 
            xStart = 0.0f;
            xEnd = m_tileSizeMM;
            yStart = (m_tileSizeMM - m_bufferVal) - (m_tileSizeMM - m_translatedPixel->getTranslatedColorValue(color));
            yEnd = m_tileSizeMM + m_bufferVal; 

            // xStart = 0.0f;
            // xEnd = m_tileSizeMM;
            // yStart = 0.0f - m_bufferVal; 
            // yEnd = (m_tileSizeMM) - m_translatedPixel->getTranslatedColorValue(color);
            
            if(not plate)
            {
                return addBlockingShapes(faceStartingNumber, 20, true, (m_tileSizeMM * m_translatedPixel->getTranslatedColorValue(color)) + m_bufferVal);
            }

            break;
        case 3:
            //full x, upper y
            //blocks top downwards
        
            xStart = 0.0f;
            xEnd = m_tileSizeMM;
            yStart = (m_tileSizeMM - m_bufferVal) - (m_tileSizeMM - m_translatedPixel->getTranslatedColorValue(color));
            yEnd = m_tileSizeMM + m_bufferVal;

            // xStart = 0.0f;
            // xEnd = m_tileSizeMM;
            // yStart = 0.0f - m_bufferVal; 
            // yEnd = (m_tileSizeMM) - m_translatedPixel->getTranslatedColorValue(color);

            if(plate)
            {
                return addBlockingShapes(faceStartingNumber, 20, true, (m_tileSizeMM * m_translatedPixel->getTranslatedColorValue(color)) + m_bufferVal);
            }
            break;
    }

    return generateOBJData(faceStartingNumber, xStart, xEnd, yStart, yEnd);
}

std::string CMYKTile::toString() const
{
    std::ostringstream ss;
    ss << "CMYKTile - x:" << getX() << " y:" << getY() << "\n" << m_translatedPixel->toString();
    return ss.str();
}
