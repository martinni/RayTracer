#include <algorithm>
#include <limits>

#include "raytracer.h"
#include "vector.h"

namespace
{

Pixel trace(const Point &origin, const Vec3 &ray, const Light &light,
            const std::vector<std::shared_ptr<Object>> &objects)
{
    float nearestObjectDist = std::numeric_limits<float>::max();
    std::shared_ptr<Object> nearestObject = nullptr;
    std::optional<Intersection> nearestObjectIntersection;

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
                nearestObjectIntersection = intersection;
            }
        }
    }

    if (!nearestObject)
    {
        return Pixel{Color{0, 0, 0}};
    }

    (void)light;
    return Pixel{nearestObject->color};
    /*
    Vec3 shadowRay = Vec3(light.position - nearestObjectIntersection.value().p);
    bool isInShadow = false;
    for (const auto &object : objects)
    {
        if (object->getIntersectionWithRay(shadowRay, origin).has_value())
        {
            isInShadow = true;
            break;
        }
    }

    if (isInShadow)
    {
        return Pixel{Color{0, 0, 0}};
    }

    Color color = nearestObject->color * light.brightness;
    return Pixel{color};
    */
}

} // namespace

std::vector<Pixel> renderScene(const std::vector<std::shared_ptr<Object>> &objects,
                               const Light &light, unsigned int width, unsigned int height)
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
            pixels.push_back(trace(Point(0), ray, light, objects));
        }
    }

    return pixels;
}
