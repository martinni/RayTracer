#include <algorithm>
#include <limits>
#include <variant>

#include "raytracer.h"
#include "vector.h"

namespace
{

struct IntersectedObject
{
    std::shared_ptr<Object> object;
    Intersection intersection;
};

struct Options
{
    Color backgroundColor;
    unsigned int recursionMaxDepth;
};

std::optional<IntersectedObject>
getNearestObject(const Point &origin, const Vec3 &rayDir,
                 const std::vector<std::shared_ptr<Object>> &objects)
{
    float nearestObjectDist = std::numeric_limits<float>::max();
    std::shared_ptr<Object> nearestObject = nullptr;
    std::optional<Intersection> nearestObjectIntersection;

    for (const auto &object : objects)
    {
        std::optional<Intersection> intersection = object->getIntersectionWithRay(rayDir, origin);
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
        return std::nullopt;
    }

    return IntersectedObject{nearestObject, nearestObjectIntersection.value()};
}

Pixel castRay(const Point &origin, const Vec3 &rayDir,
              const std::vector<std::shared_ptr<Object>> &objects,
              const std::vector<std::shared_ptr<Light>> &lights, const Options &options,
              unsigned int &depth)
{
    if (depth > options.recursionMaxDepth)
    {
        return Pixel{options.backgroundColor};
    }

    std::optional<IntersectedObject> nearestObject = getNearestObject(origin, rayDir, objects);
    if (!nearestObject)
    {
        return Pixel{options.backgroundColor};
    }

    const auto &object = nearestObject.value().object;
    const auto &nearestObjectMaterialProperties = object->materialProperties;

    // Will implement the visitor after learning about shading and illumination
    Color pixelColor = std::visit(
        [&object](const auto &properties)
        {
            using T = std::decay_t<decltype(properties)>;
            if constexpr (std::is_same_v<T, DiffuseProperties>)
            {
                return object->color;
            }
            else if constexpr (std::is_same_v<T, MirrorProperties>)
            {
                return object->color;
            }
            else if constexpr (std::is_same_v<T, TransparentProperties>)
            {
                return object->color;
            }
            else
            {
                std::cerr << "unknown material properties" << std::endl;
            }
        },
        nearestObjectMaterialProperties);

    (void)lights;

    return Pixel{pixelColor};
}

} // namespace

std::vector<Pixel> renderScene(const std::vector<std::shared_ptr<Object>> &objects,
                               const std::vector<std::shared_ptr<Light>> &lights,
                               unsigned int width, unsigned int height)
{
    std::vector<Pixel> pixels;
    pixels.reserve(width * height);

    float invWidth = 1 / float(width);
    float invHeight = 1 / float(height);

    float fov = 30;
    float aspectRatio = width / float(height);
    float angle = tan(M_PI * 0.5 * fov / 180.);
    Options options{Color{0, 0, 0}, 5};

    // Compute each pixel color
    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectRatio;
            float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
            Vec3 cameraRay(xx, yy, -1);
            cameraRay.normalize();

            unsigned int depth = 0;
            pixels.push_back(castRay(Point(0), cameraRay, objects, lights, options, depth));
        }
    }

    return pixels;
}
