#include <cmath>

#include "object.h"

Object::Object(const Color &p_color, const MaterialProperties &p_materialProperties)
    : color(p_color), materialProperties(p_materialProperties)
{
}

Object::~Object() {}

Sphere::Sphere(const Point &p_center, float p_radius, const Color &p_color,
               const MaterialProperties &p_materialProperties)
    : Object(p_color, p_materialProperties), center(p_center), radius(p_radius)
{
}

std::optional<Intersection> Sphere::getIntersectionWithRay(const Ray &ray) const
{
    /*
        Line equation:
            - x = origin.x + (ray.x - origin.x) * t
            - y = origin.y + (ray.y - origin.y) * t
            - z = origin.z + (ray.z - origin.z) * t
        Sphere equation:
            (x-center.y)^2 + (y-center.y)^2 + (z-center.z)^2 = radius^2

        Substituting line values x, y and z into the equation of the sphere gives a quadratic
        equation of the form a*t^2 + b*t + c = 0 where:
            - a = (x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2
            - b = -2[(x2 - x1) * (xc - x1) + (y2 - y1) * (yc - y1) + (z2 - z1) * (zc - z1)]
            - c = (xc - x1)^2 + (yc - y1)^2 + (zc - z1)^2 - radius^2

        where (x1, y1, z1) is the origin and (x2, y2, z2) is the point defined by
        (ray.x, ray.y, ray.z)

        The solutions (when they exist) for the above are:
            t1 = (-b + sqrt(b^2 - 4 * a * c)) / (2 * a)
            t2 = (-b - sqrt(b^2 - 4 * a * c)) / (2 * a)

        When (b^2 - 4 * a * c) is :
            - positive: there are 2 solutions
            - 0: there is 1 solution
            - negative: there are no solutions
    */
    float a = pow((ray.direction.x - ray.origin.x), 2) + pow((ray.direction.y - ray.origin.y), 2) +
              pow((ray.direction.z - ray.origin.z), 2);
    float b = -2 * ((ray.direction.x - ray.origin.x) * (center.x - ray.origin.x) +
                    (ray.direction.y - ray.origin.y) * (center.y - ray.origin.y) +
                    (ray.direction.z - ray.origin.z) * (center.z - ray.origin.z));
    float c = pow((center.x - ray.origin.x), 2) + pow((center.y - ray.origin.y), 2) +
              pow((center.z - ray.origin.z), 2) - pow(radius, 2);

    // Use more stable equation to avoid catastrophic cancellation
    float discriminant = pow(b, 2) - 4 * a * c;

    if (discriminant < 0)
    {
        return std::nullopt;
    }

    // Following equations are more stable than the usual [-|+]b + sqrt(delta) / 2 * a;
    // See "catastrophic cancellation" and "loss of significance"
    float q = (b > 0) ? -0.5 * (b + sqrt(discriminant)) : -0.5 * (b - sqrt(discriminant));
    float t1 = q / a;
    float t2 = c / q;

    if (t1 < 0 && t2 < 0)
    {
        // Both intersections are behind the origin
        return std::nullopt;
    }

    // We keep whatever point is closest to the ray origin
    float t0 = t1 >= t2 ? t1 : t2;

    // Calculate intersection coordinate
    float x = ray.origin.x + ray.direction.x * t0;
    float y = ray.origin.y + ray.direction.y * t0;
    float z = ray.origin.z + ray.direction.z * t0;

    Point p(x, y, z);
    Vec3 normal = Vec3(p - center).normalize();

    return Intersection{p, normal};
}
