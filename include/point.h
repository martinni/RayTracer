#pragma once

#include <vector.h>

class Point
{
  public:
    float x, y, z;

    Point() : x(0), y(0), z(0) {}
    Point(float xx) : x(xx), y(xx), z(xx) {}
    Point(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

    Point operator+(const Point &v) const { return Point(x + v.x, y + v.y, z + v.z); }

    Point operator-(const Point &v) const { return Point(x - v.x, y - v.y, z - v.z); }

    Point operator+(const Vec3f &v) const { return Point(x + v.x, y + v.y, z + v.z); }
};
