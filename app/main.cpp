#include <iostream>
#include <stdlib.h>
#include <vector>

#include "matrix.h"
#include "pixel.h"
#include "ppmFile.h"
#include "raytracer.h"
#include "vector.h"

const int IMG_WIDTH = 1024;
const int IMG_HEIGHT = 788;
const int COLOR_RANGE = 255;

int main()
{
    std::cout << "Hello World" << std::endl;

    Vec3 vec(1, 2, 3);
    Matrix44 mat(1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4);

    PpmFile ppmFile("./scene.ppm", IMG_WIDTH, IMG_HEIGHT, COLOR_RANGE);

    /*(std::vector<Pixel> pixels;
    for (int i = 0; i < 10000; i++)
    {
        pixels.push_back(Pixel{static_cast<float>(i) / 10000, 1 - static_cast<float>(i) / 10000,
                               static_cast<float>(0.5)});
    }
    ppmFile.writePixels(pixels);*/
}
