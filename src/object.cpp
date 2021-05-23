#include <cmath>

#include "object.h"

Object::~Object() {}

std::optional<Intersection> Sphere::getIntersectionWithRay(const Vec3f &ray,
                                                           const Point origin) const
{
    Vec3f L(center.x - origin.x, center.y - origin.y, center.z - origin.z);
    float tca = L.dotProduct(ray);

    float d2 = L.dotProduct(L) - tca * tca;
    if (d2 > radius)
    {
        return std::nullopt;
    }

    float thc = sqrt(float(float(pow(radius, 2))) - d2);
    float t0 = tca - thc;

    Point hitPoint = origin + ray * t0;

    Point diff = hitPoint - center;
    Vec3f hitNormal = Vec3f(diff.x, diff.y, diff.z);
    hitNormal.normalize();

    return Intersection{hitPoint, hitNormal};
}
