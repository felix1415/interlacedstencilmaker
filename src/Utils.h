//Copyright (c) 2021 Alex Gray

#include <bitmap_image.hpp>

class Utils
{
    public:
        static std::string colorToString(bitmap_image::color_plane color)
        {
            switch(color)
            {
                case bitmap_image::color_plane::red_plane:
                    return "red";
                case bitmap_image::color_plane::green_plane:
                    return "green";
                case bitmap_image::color_plane::blue_plane:
                    return "blue";
                default:
                    return "NotAColor";
            }
        }

        static std::string colorToString(int color)
        {
            return colorToString((bitmap_image::color_plane)color);
        }

};