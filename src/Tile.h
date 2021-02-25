//Copyright (c) 2021 Alex Gray

#include <iostream>
#include <string>
#include "Pixel.h"

namespace cics
{
    enum TileType
    {
        InnerTile,
        BorderTile,
    };

    /* Tile will generate the obj values for the space it is to occupy in 
    the stencil based on the strength of the colour it is representing. 
    It doesn't need to know which colour, only how strong that colour 
    should appear*/
    class Tile
    {
        static const int STEPS = 4;

        public:
            Tile(const std::vector<Pixel> & pixels, const bitmap_image::color_plane color);
            void getOBJ();

        private:
            void calculateStrength();
            uint8_t getColorValue(const rgb_t colourObj);

        private:
            std::vector<Pixel> m_pixels;
            bitmap_image::color_plane m_color;
            uint8_t m_strength;
            

    };
}