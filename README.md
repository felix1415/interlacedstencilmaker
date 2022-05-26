# interlacedstencilmaker
This program will take a BMP image as input and produce 6 OBJ files that relate to each of the 3 primary colours, 2 for each. Each OBJ is to be 3D printed and used as a spray stencil against black card. 

2 stencils are required for each colour as supporting bars are inserted into the stencil on alternating lines, the 'top' stencil will be even numbered Y lines, the 'bottom' stencil will be odd numbered Y lines. This makes the stencils themselves interlaced.

I use a 0.4mm nozzle, smaller will increase the time required to print, larger will decrease the maximum number of pixels you can represent. A single stencil will take about 12 hours, so 3 total days to print all 6 OBJ files for a given image. These numbers are representative of 220mmx220mm stencil (the max size I can print on an Ender 3.)

The top red stencil for the 'Small Nasa Logo':
![OBJ Example](https://github.com/felix1415/interlacedstencilmaker/blob/main/obj_ss_example.JPG)

![Small Nasa Logo](https://github.com/felix1415/interlacedstencilmaker/blob/main/smallnasa.bmp)

### Running

Standard example:
./stencil_maker -i sample.bmp -W 220 -H 220 --grayscale

### Testing

cd test/
make
./ism_test

### Help
3 Colour Interlaced Stencil Creator
A small utility for turning bitmap images into Interlaced stencils
Usage:
  Interlaced Stencil Maker [OPTION...]

  -d, --debug       Debugging mode
  -g, --grayscale   Generate grayscale stencil instead of an RGB collection
                    of stencils
  -w, --wrgb        Generate an RGB set of stencils to complement the
                    grayscale
  -W, --width arg   Width of image
  -H, --height arg  Height of image
  -i, --input arg   Input file
  -o, --output arg  Output folder (default: ISM)

Todo:
Upload the Cura settings for the 3D printer. 
