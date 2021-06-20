#include <iostream>
#include <memory>
#include <stdlib.h>
#include <vector>

#include "matrix.h"
#include "object.h"
#include "pixel.h"
#include "ppmFile.h"
#include "raytracer.h"
#include "vector.h"

const unsigned int IMG_WIDTH = 1024;
const unsigned int IMG_HEIGHT = 788;
const unsigned int COLOR_RANGE = 255;

int main()
{
    PpmFile ppmFile("./scene.ppm", IMG_WIDTH, IMG_HEIGHT, COLOR_RANGE);

    std::vector<std::shared_ptr<Object>> objects = {
        std::make_shared<Sphere>(Point(1, 2, -15), 0.5, Color{1, 0, 0}, DiffuseProperties{}),
        std::make_shared<Sphere>(Point(0.0, 0, -20), 2, Color{0, 1, 0}, DiffuseProperties{}),
        std::make_shared<Sphere>(Point(5.0, -1, -15), 2, Color{0, 0, 0.5}, DiffuseProperties{}),
        std::make_shared<Sphere>(Point(-5.5, 0, -15), 3, Color{0.5, 0.5, 0.5},
                                 DiffuseProperties{})};

    std::vector<std::shared_ptr<Light>> lights = {
        std::make_shared<Light>(Light{Point(7, 7, 0), 0.7})};

    std::vector<Pixel> pixels = renderScene(objects, lights, IMG_WIDTH, IMG_HEIGHT);
    ppmFile.writePixels(pixels);
}
