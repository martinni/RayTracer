#pragma once

#include <memory>
#include <vector>

#include "object.h"
#include "pixel.h"
#include "point.h"

struct Light
{
    Point position;
    float brightness;
};

std::vector<Pixel> renderScene(const std::vector<std::shared_ptr<Object>> &objects,
                               const std::vector<std::shared_ptr<Light>> &lights,
                               unsigned int width, unsigned int height);
