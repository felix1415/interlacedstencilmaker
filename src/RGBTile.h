//Copyright (c) 2022 Alex Gray

#pragma once
#include "Tile.h"

/* RGBTile will generate the obj values for the space it is to occupy in 
the stencil based on the strength of the colour it is representing. 
It doesn't need to know which colour, only how strong that colour 
should appear*/
class RGBTile : public Tile
{

    public: //inner grouped color RGBTile
        RGBTile(std::unique_ptr<TranslatedPixel> && pixel);

    public:
        virtual OBJData getOBJData(int & faceStartingNumber, const int color) const override;
        virtual std::string toString() const;
        // virtual int numberOfColors() const;
};