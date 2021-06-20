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

    float z = pow(b, 2) - 4 * a * c;

    if (z < 0)
    {
        return std::nullopt;
    }
    float t1 = -b + sqrt(z) / 2 * a;
    float t2 = -b - sqrt(z) / 2 * a;

    // Calculate 2 intersections
    float x1 = ray.origin.x + ray.direction.x * t1;
    float y1 = ray.origin.y + ray.direction.y * t1;
    float z1 = ray.origin.z + ray.direction.z * t1;

    float x2 = ray.origin.x + ray.direction.x * t2;
    float y2 = ray.origin.y + ray.direction.y * t2;
    float z2 = ray.origin.z + ray.direction.z * t2;

    // We return whatever point is closest to the ray origin
    Point p = x1 >= ray.origin.x && x1 < x2 ? Point(x1, y1, z1) : Point(x2, y2, z2);

    Vec3 normal = Vec3(p - center).normalize();

    return Intersection{p, normal};
}
