//Copyright (c) 2022 Alex Gray

#pragma once
#include "Tile.h"

/* CMYKTile will generate the obj values for the space it is to occupy in 
the stencil based on the strength of the colour it is representing. 
It doesn't need to know which colour, only how strong that colour 
should appear*/
class CMYKTile : public Tile
{

    public: //inner grouped color CMYKTile
        CMYKTile(std::unique_ptr<TranslatedPixel> && pixel);

    private:
        OBJData generateOBJData(int & faceStartingNumber, const float xStart,  const float xEnd,  const float yStart,  const float yEnd) const;

    public:
        OBJData getOBJData(int & faceStartingNumber, const int color) const override;
        std::string toString() const;
};