#pragma once

#include <optional>
#include <variant>

#include "pixel.h"
#include "point.h"
#include "vector.h"

struct Intersection
{
    Point p;
    Vec3 normal;
};

struct DiffuseProperties
{
    Color color;
    float Ks, Kd; // Phong model parameters
    float specularExponent;
};

struct MirrorProperties
{
    float indexOfRefraction;
};

struct TransparentProperties
{
    float indexOfRefraction;
};

using MaterialProperties = std::variant<DiffuseProperties, MirrorProperties, TransparentProperties>;

class Object
{
  public:
    Color color;
    MaterialProperties materialProperties;

    Object(const Color &color, const MaterialProperties &materialProperties);
    virtual ~Object();
    virtual std::optional<Intersection> getIntersectionWithRay(const Vec3 &ray,
                                                               const Point origin) const = 0;
};

class Sphere : public Object
{
  public:
    Sphere(const Point &center, float radius, const Color &color,
           const MaterialProperties &materialProperties);
    virtual std::optional<Intersection> getIntersectionWithRay(const Vec3 &ray,
                                                               const Point origin) const override;

  private:
    Point center;
    float radius;
};
