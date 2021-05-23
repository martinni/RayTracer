#pragma once

#include <optional>

#include "pixel.h"
#include "point.h"
#include "vector.h"

struct Intersection
{
    Point p;
    Vec3f normal;
};

class Object
{
  public:
    Color color;

    virtual ~Object();
    virtual std::optional<Intersection> getIntersectionWithRay(const Vec3f &ray,
                                                               const Point origin) const = 0;
};

class Sphere : public Object
{
  public:
    virtual std::optional<Intersection> getIntersectionWithRay(const Vec3f &ray,
                                                               const Point origin) const override;

  private:
    Point center;
    float radius;
};
