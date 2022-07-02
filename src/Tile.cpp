//Copyright (c) 2021 Alex Gray

#include "Tile.h"
#include "Utils.h"
#include <bitmap_image.hpp>
#include <math.h>
#include <iostream>
#include <sstream>

Tile::Tile(std::unique_ptr<TranslatedPixel> && pixel):
Position(pixel->getX(), pixel->getY()), // THIS WILL CAUSE PROBLEMS - SOLVE IT
m_translatedPixel(std::move(pixel)),
m_tileSizeMM(m_translatedPixel->getTileSizeMM()),
m_type(TileType::innerTile)
{
    //our tile is going to fit into a large grid of tiles, if we give 
    //is a pixel, it's an inner tile, we plus one to the inner tiles 
    //so that border tiles can fill their space
}

Tile::Tile(const float x, const float y, const float tileSizeMM):
Position(x, y),
m_tileSizeMM(tileSizeMM),
m_type(TileType::borderTile)
{
}

OBJData Tile::getOBJData(int & faceStartingNumber, const int color) const
{
    const size_t numberOfColors = m_translatedPixel->getColorArraySize();
    float bufferVal = Utils::getBufferOBJValue(m_tileSizeMM);
    std::vector<vertices> verticesVec;
    std::vector<faces> facesVec;
    verticesVec.reserve(INNER_TILE_VERTS * numberOfColors); // 8 vertices * 3 colors
    facesVec.reserve(INNER_TILE_FACES * numberOfColors);  

    float xStart = 0.0f;
    float xEnd = m_tileSizeMM / numberOfColors;
    float yStart = 0.0f; // always 0, only end will change (pixel strength)
    float yEnd = 0.0f + bufferVal; // strength of color

    
    // for(const auto & color : m_translatedPixel.getColorArray())
    for(size_t i = 0; i < numberOfColors; i++)
    {
        TileOptions tOptions = TileOptions::none;
        //y height is always the length of the tile, unless the TP has a 
        //value to reduce it (allowing color through)
        if(((bitmap_image::color_plane)i == color))
        {
            //start ourselves a little to the left to account for the empty space buffer
            xStart -= bufferVal;
            xEnd += bufferVal;
            yEnd = (m_tileSizeMM - m_translatedPixel->getTranslatedColorValue((bitmap_image::color_plane)i));
            //if our translated color is at tileSize intensity, we can skip over it, the tile is too small to print
            //better than a equality operation, we're asking if it's less than the smallest possible value
            if(yEnd < (m_tileSizeMM / (m_translatedPixel->getSteps() + 1)) )
            {
                xStart = xEnd;
                xEnd += m_tileSizeMM / numberOfColors;
                tOptions = TileOptions::omitBottom;
                continue;
            }
        }
        else
        {
            yEnd = m_tileSizeMM + bufferVal;
            tOptions = TileOptions::omitEnds;
            //OPTIMISATION, if we're one of the colors that aren't on this stencil, we can just 
            //extend our width out to cover the other color that isn't on this stencil and skip
            //the addition of the other vertices, this saves us a bunch of vertices (50%)
            if((color == bitmap_image::color_plane::red_plane and i == 0)) 
            {   
                //red is at the end, so here we can extend the first part of the tile from blue over green 
                xEnd += (m_tileSizeMM / numberOfColors) - bufferVal;
            }
            else if((color == bitmap_image::color_plane::blue_plane and i == 1)) 
            {   
                //we want a bit of buffer for the empty tile, so we get that from the space buffer
                xEnd += (m_tileSizeMM / numberOfColors) - bufferVal;
            }
            //green is inbetween red and blue, so the green block over laps from red to blue 
            //accross tile boundaries, at the end of the tile, however, on the first tile (we
            //start at 1 on inner tiles) we still need to have the wall because we don't do the 
            //optimisation until the end of the tile
            //this is the first green tile on the line, we still need to put the blocking tile in at the start
            else if((color == bitmap_image::color_plane::green_plane) and getX() == 1 and i == 0)
            {   
                //just allow the buffer to end before the start of the color tile
                xEnd -=  bufferVal;
            }
            //this is the wide blocking tile
            else if((color == bitmap_image::color_plane::green_plane and i == 2)) 
            {   
                //green is in the middle, so extend the blocking tile into the next tile
                xEnd += (m_tileSizeMM / numberOfColors) - bufferVal;
            }
            //this is a skipping tile, still extend end for the actaul color to use
            //green is in the middle, so extend the blocking tile into the next color
            else if((color == bitmap_image::color_plane::green_plane and i == 0)) 
            {   
                xEnd += (m_tileSizeMM / numberOfColors);
                continue;
            }
            else
            {
                //we're the colour being optimised for, continue
                continue;
            }            
        }
        
        std::vector<vertices> colorVerts = getVertices(xStart, xEnd, yStart, yEnd);
        verticesVec.insert(std::end(verticesVec), std::begin(colorVerts), std::end(colorVerts));
        
        std::vector<faces> colorFaces = getFaces(faceStartingNumber, tOptions);
        facesVec.insert(std::end(facesVec), std::begin(colorFaces), std::end(colorFaces));

        //next color prep
        xStart = xEnd;
        xEnd += m_tileSizeMM / 3;
    }

    return std::make_pair(verticesVec, facesVec);
}

OBJData Tile::addBlock(float xStart, float xEnd, float yStart, float yEnd, int & faceStartingNumber) const
{
    // std::cout << "ADDB "<< xStart << " " << xEnd << " " << yStart << " " << yEnd << std::endl;
    return std::make_pair(getVertices(xStart, xEnd, yStart, yEnd), getFaces(faceStartingNumber));
}

std::vector<vertices> Tile::getVertices(float xStart, float xEnd, float yStart, float yEnd) const
{
    float xVert = ((float)getX() * m_tileSizeMM);
    float yVert = ((float)getY() * m_tileSizeMM);

    std::vector<vertices> verticesToReturn = {
        {xVert + xStart,     yVert + yStart,      0},
        {xVert + xEnd,       yVert + yStart,      0},
        {xVert + xEnd,       yVert + yEnd,        0},
        {xVert + xStart,     yVert + yEnd,        0},
        {xVert + xStart,     yVert + yStart,      STENCIL_THICKNESS},
        {xVert + xEnd,       yVert + yStart,      STENCIL_THICKNESS},
        {xVert + xEnd,       yVert + yEnd,        STENCIL_THICKNESS},
        {xVert + xStart,     yVert + yEnd,        STENCIL_THICKNESS},
    };

    return verticesToReturn;
}

std::vector<faces> Tile::getFaces(int & faceStartingNumber, const TileOptions) const
{
    std::vector<faces> facesToReturn;
    facesToReturn.push_back({uint32_t(1+faceStartingNumber), uint32_t(2+faceStartingNumber), uint32_t(3+faceStartingNumber), uint32_t(4+faceStartingNumber)});
    facesToReturn.push_back({uint32_t(5+faceStartingNumber), uint32_t(6+faceStartingNumber), uint32_t(7+faceStartingNumber), uint32_t(8+faceStartingNumber)});
    
    //NOTE: tileOptions was removed because they didn't allow cura to slice properly, more research
    // needs to be done on how this affect cura

    //we can remove the top face of vertices from the blocking inner tile as they are 
    //always up against the outer tile (bar) bottom vertices
    // if(tileOptions != TileOptions::omitEnds) //we can omit this face, the outerTile will cover it always
    {
        facesToReturn.push_back({uint32_t(3+faceStartingNumber), uint32_t(4+faceStartingNumber), uint32_t(8+faceStartingNumber), uint32_t(7+faceStartingNumber)});//top
    }

    //we can remove the bottom face of vertices from each inner tile as they
    // are always up against the outer tile (bar)
    // if(tileOptions != TileOptions::omitBottom and tileOptions != TileOptions::omitEnds )
    {
        facesToReturn.push_back({uint32_t(1+faceStartingNumber), uint32_t(2+faceStartingNumber), uint32_t(6+faceStartingNumber), uint32_t(5+faceStartingNumber)});//bottom
    }

    //we can remove the side face of vertices from both sides of the active color 
    //as they are always up against the blocking inner tile (bar)
    // if(tileOptions != TileOptions::omitSides)
    {
        //both sides of the tile
        facesToReturn.push_back({uint32_t(2+faceStartingNumber), uint32_t(3+faceStartingNumber), uint32_t(7+faceStartingNumber), uint32_t(6+faceStartingNumber)}); // right side
        facesToReturn.push_back({uint32_t(4+faceStartingNumber), uint32_t(1+faceStartingNumber), uint32_t(5+faceStartingNumber), uint32_t(8+faceStartingNumber)}); // left side    
    }
    

    faceStartingNumber += 8;

    return facesToReturn;
}

std::vector<faces> Tile::getTriangleFaces(int & faceStartingNumber) const
{
    std::vector<faces> facesToReturn;
    facesToReturn.push_back({uint32_t(1+faceStartingNumber), uint32_t(2+faceStartingNumber), uint32_t(3+faceStartingNumber), 0});
    facesToReturn.push_back({uint32_t(4+faceStartingNumber), uint32_t(5+faceStartingNumber), uint32_t(6+faceStartingNumber), 0});
    facesToReturn.push_back({uint32_t(5+faceStartingNumber), uint32_t(1+faceStartingNumber), uint32_t(3+faceStartingNumber), 0});
    facesToReturn.push_back({uint32_t(5+faceStartingNumber), uint32_t(3+faceStartingNumber), uint32_t(6+faceStartingNumber), 0});
    facesToReturn.push_back({uint32_t(2+faceStartingNumber), uint32_t(1+faceStartingNumber), uint32_t(5+faceStartingNumber), 0});
    facesToReturn.push_back({uint32_t(2+faceStartingNumber), uint32_t(5+faceStartingNumber), uint32_t(4+faceStartingNumber), 0});
    facesToReturn.push_back({uint32_t(2+faceStartingNumber), uint32_t(4+faceStartingNumber), uint32_t(6+faceStartingNumber), 0});
    facesToReturn.push_back({uint32_t(2+faceStartingNumber), uint32_t(6+faceStartingNumber), uint32_t(3+faceStartingNumber), 0});

    faceStartingNumber += 6;

    return facesToReturn;
}



std::string Tile::toString() const
{
    std::ostringstream ss;
    ss << "Tile - x:" << getX() << " y:" << getY() << "\n" << m_translatedPixel->toString();
    return ss.str();
}
