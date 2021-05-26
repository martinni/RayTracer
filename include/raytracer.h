#pragma once

#include <memory>
#include <vector>

#include "object.h"
#include "pixel.h"

std::vector<Pixel> renderScene(const std::vector<std::shared_ptr<Object>> &objects,
                               unsigned int width, unsigned int height);
