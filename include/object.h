#pragma once

#include <optional>

#include "pixel.h"
#include "point.h"
#include "vector.h"

struct Intersection
{
    Point p;
    Vec3 normal;
};

class Object
{
  public:
    Color color;

    virtual ~Object();
    virtual std::optional<Intersection> getIntersectionWithRay(const Vec3 &ray,
                                                               const Point origin) const = 0;
};

class Sphere : public Object
{
  public:
    Sphere(const Point &center, float radius);
    virtual std::optional<Intersection> getIntersectionWithRay(const Vec3 &ray,
                                                               const Point origin) const override;

  private:
    Point center;
    float radius;
};
