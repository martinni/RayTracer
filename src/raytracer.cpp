#include <algorithm>
#include <limits>
#include <variant>

#include "raytracer.h"
#include "vector.h"

namespace
{

// Using the same material parameters for every objects for now
const float SPECULAR_EXPONENT = 25;
const float KD = 0.8;
const float KS = 0.2;

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

inline float degree2radian(float degree) { return degree * M_PI / 180; }

Vec3 getReflectionDirection(const Vec3 &lightDir, const Vec3 &normal)
{
    return lightDir - normal * 2 * lightDir.dotProduct(normal);
}

std::optional<IntersectedObject>
getNearestObject(const Ray &ray, const std::vector<std::shared_ptr<Object>> &objects)
{
    float nearestObjectDist = std::numeric_limits<float>::max();
    std::shared_ptr<Object> nearestObject = nullptr;
    std::optional<Intersection> nearestObjectIntersection;

    for (const auto &object : objects)
    {
        std::optional<Intersection> intersection = object->getIntersectionWithRay(ray);
        if (intersection.has_value())
        {
            float intersectionDist = Vec3(intersection.value().p - ray.origin).norm();
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

Color shade(const IntersectedObject &intersectedObject, const Ray &ray,
            const std::vector<std::shared_ptr<Object>> &objects,
            const std::vector<std::shared_ptr<Light>> &lights)
{
    const Intersection &intersection = intersectedObject.intersection;

    float lightIntensity = 0;
    float specularColor = 0;

    // Filter out intersected object from the object array so that we don't intersect it with itself
    std::vector<std::shared_ptr<Object>> otherObjects;
    std::copy_if(objects.begin(), objects.end(), std::back_inserter(otherObjects),
                 [&intersectedObject](const std::shared_ptr<Object> &obj)
                 { return obj != intersectedObject.object; });

    for (const std::shared_ptr<Light> &light : lights)
    {
        Ray shadowRay{intersection.p, Vec3(light->position - intersection.p).normalize()};
        std::optional<IntersectedObject> nearestObject = getNearestObject(shadowRay, otherObjects);
        if (nearestObject.has_value())
        {
            float shadowObjectToLightDist =
                Vec3(nearestObject.value().intersection.p - light->position).norm();
            float objectToLightDist = Vec3(intersection.p - light->position).norm();
            if (shadowObjectToLightDist < objectToLightDist)
            {
                continue;
            }
        }
        lightIntensity += (light->brightness *
                           std::max(0.f, shadowRay.direction.dotProduct(intersection.normal)));
        Vec3 reflectionDirection =
            getReflectionDirection(shadowRay.direction * -1, intersection.normal);
        specularColor +=
            powf(std::max(0.f, -reflectionDirection.dotProduct(ray.direction)), SPECULAR_EXPONENT) *
            light->brightness;
    }
    return intersectedObject.object->color * lightIntensity * KD + specularColor * KS;
}

Pixel castRay(const Ray &ray, const std::vector<std::shared_ptr<Object>> &objects,
              const std::vector<std::shared_ptr<Light>> &lights, const Options &options)
{
    std::optional<IntersectedObject> nearestObject = getNearestObject(ray, objects);
    if (!nearestObject)
    {
        return Pixel{options.backgroundColor};
    }

    Color pixelColor = shade(nearestObject.value(), ray, objects, lights);
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

    float fieldOfView = 30;
    float aspectRatio = width / float(height);
    float angle = tan(degree2radian(0.5 * fieldOfView));
    Options options{Color{0, 0, 0}, 5};

    // Compute each pixel color
    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectRatio;
            float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;

            Point cameraRayOrigin{Point(0)};
            Vec3 cameraRayDirection = Vec3(xx, yy, -1) - cameraRayOrigin;
            cameraRayDirection.normalize();
            Ray cameraRay{cameraRayOrigin, cameraRayDirection};

            pixels.push_back(castRay(cameraRay, objects, lights, options));
        }
    }

    return pixels;
}
