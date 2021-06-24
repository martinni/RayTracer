#include <iostream>
#include <memory>
#include <random>
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

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    std::uniform_real_distribution<> dis2(-0.7, 0.7);

    std::vector<std::shared_ptr<Object>> objects;
    const int numSpheres = 10;
    for (int i = 0; i < numSpheres; i++)
    {
        Point randomPosition(static_cast<float>(dis2(gen)), static_cast<float>(dis2(gen)),
                             -1 * static_cast<float>(dis(gen)) * 10);
        float radius = static_cast<float>(dis(gen)) * 0.5;
        Color color{static_cast<float>(dis(gen)), static_cast<float>(dis(gen)),
                    static_cast<float>(dis(gen))};
        objects.push_back(
            std::make_shared<Sphere>(randomPosition, radius, color, DiffuseProperties{}));
    }

    std::vector<std::shared_ptr<Light>> lights = {
        std::make_shared<Light>(Light{Point(10, 4, -12), 1})};

    std::vector<Pixel> pixels = renderScene(objects, lights, IMG_WIDTH, IMG_HEIGHT);
    ppmFile.writePixels(pixels);
}
