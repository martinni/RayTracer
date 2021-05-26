#include <algorithm>
#include <limits>

#include "raytracer.h"
#include "vector.h"

namespace
{

Pixel trace(const Point &origin, const Vec3 &ray,
            const std::vector<std::shared_ptr<Object>> &objects)
{
    float nearestObjectDist = std::numeric_limits<float>::max();
    std::shared_ptr<Object> nearestObject = nullptr;

    for (const auto &object : objects)
    {
        std::optional<Intersection> intersection = object->getIntersectionWithRay(ray, origin);
        if (intersection.has_value())
        {
            float intersectionDist = Vec3(intersection.value().p - origin).norm();
            if (intersectionDist < nearestObjectDist)
            {
                nearestObjectDist = intersectionDist;
                nearestObject = object;
            }
        }
    }

    if (nearestObject)
    {
        return Pixel{Color{0.5, 1, 0.5}};
    }

    return Pixel{Color{0, 0, 0}};
}

} // namespace

std::vector<Pixel> renderScene(const std::vector<std::shared_ptr<Object>> &objects,
                               unsigned int width, unsigned int height)
{
    std::vector<Pixel> pixels;
    pixels.reserve(width * height);

    float invWidth = 1 / float(width);
    float invHeight = 1 / float(height);

    float fov = 30;
    float aspectRatio = width / float(height);
    float angle = tan(M_PI * 0.5 * fov / 180.);

    // Trace rays
    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectRatio;
            float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
            Vec3 ray(xx, yy, -1);
            ray.normalize();
            pixels.push_back(trace(Point(0), ray, objects));
        }
    }

    return pixels;
}
