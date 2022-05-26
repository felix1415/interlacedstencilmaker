//Copyright (c) 2021 Alex Gray

#include "RGBTile.h"
#include "Utils.h"
#include <bitmap_image.hpp>
#include <math.h>
#include <iostream>
#include <sstream>

RGBTile::RGBTile(std::unique_ptr<TranslatedPixel> && pixel):
Tile(std::move(pixel))
{
}

std::pair<std::vector<vertices>,std::vector<faces>> RGBTile::getOBJData(int & faceStartingNumber, const int color) const
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

    for(size_t i = 0; i < numberOfColors; i++)
    {
        TileOptions tOptions = TileOptions::none;
        //y height is always the length of the RGBTile, unless the TP has a 
        //value to reduce it (allowing color through)
        if(((bitmap_image::color_plane)i == color))
        {
            //start ourselves a little to the left to account for the empty space buffer
            xStart -= bufferVal;
            xEnd += bufferVal;
            yEnd = (m_tileSizeMM - m_translatedPixel->getTranslatedColorValue((bitmap_image::color_plane)i)) + bufferVal;
            //if our translated color is at RGBTileSize intensity, we can skip over it, the RGBTile is too small to print
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
                //red is at the end, so here we can extend the first part of the RGBTile from blue over green 
                xEnd += (m_tileSizeMM / numberOfColors) - bufferVal;
            }
            else if((color == bitmap_image::color_plane::blue_plane and i == 1)) 
            {   
                //we want a bit of buffer for the empty RGBTile, so we get that from the space buffer
                xEnd += (m_tileSizeMM / numberOfColors) - bufferVal;
            }
            //green is inbetween red and blue, so the green block over laps from red to blue 
            //accross RGBTile boundaries, at the end of the RGBTile, however, on the first RGBTile (we
            //start at 1 on inner RGBTiles) we still need to have the wall because we don't do the 
            //optimisation until the end of the RGBTile
            //this is the first green RGBTile on the line, we still need to put the blocking RGBTile in at the start
            else if((color == bitmap_image::color_plane::green_plane) and getX() == 1 and i == 0)
            {   
                //just allow the buffer to end before the start of the color RGBTile
                xEnd -=  bufferVal;
            }
            //this is the wide blocking RGBTile
            else if((color == bitmap_image::color_plane::green_plane and i == 2)) 
            {   
                //green is in the middle, so extend the blocking RGBTile into the next RGBTile
                xEnd += (m_tileSizeMM / numberOfColors) - bufferVal;
            }
            //this is a skipping RGBTile, still extend end for the actaul color to use
            //green is in the middle, so extend the blocking RGBTile into the next color
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

std::string RGBTile::toString() const
{
    std::ostringstream ss;
    ss << "RGBTile - x:" << getX() << " y:" << getY() << "\n" << m_translatedPixel->toString();
    return ss.str();
}

// int RGBTile::numberOfColors() const
// {
//     return m_translatedPixel->getColorArraySize();
// }
