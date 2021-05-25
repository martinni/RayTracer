#pragma once

#include <cmath>
#include <iostream>

#include "point.h"

// Implementation copied from https://www.scratchapixel.com/
class Vec3
{
  public:
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float xx) : x(xx), y(xx), z(xx) {}
    Vec3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
    Vec3(Point p) : x(p.x), y(p.y), z(p.z) {}

    Vec3 operator+(const Vec3 &v) const { return Vec3(x + v.x, y + v.y, z + v.z); }

    Vec3 operator-(const Vec3 &v) const { return Vec3(x - v.x, y - v.y, z - v.z); }

    Vec3 operator*(const float &r) const { return Vec3(x * r, y * r, z * r); }

    float dotProduct(const Vec3 &v) const { return x * v.x + y * v.y + z * v.z; }

    Vec3 crossProduct(const Vec3 &v) const
    {
        return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    float norm() const { return x * x + y * y + z * z; }

    float length() const { return sqrt(norm()); }

    // The next two operators are sometimes called access operators or
    // accessors. The Vec coordinates can be accessed that way v[0], v[1], v[2],
    // rather than using the more traditional form v.x, v.y, v.z. This useful
    // when vectors are used in loops: the coordinates can be accessed with the
    // loop index (e.g. v[i]).
    //[/comment]
    const float &operator[](uint8_t i) const { return (&x)[i]; }
    float &operator[](uint8_t i) { return (&x)[i]; }

    Vec3 &normalize()
    {
        float n = norm();
        if (n > 0)
        {
            float factor = 1 / sqrt(n);
            x *= factor;
            y *= factor;
            z *= factor;
        }

        return *this;
    }

    friend std::ostream &operator<<(std::ostream &s, const Vec3 &v)
    {
        return s << '(' << v.x << ' ' << v.y << ' ' << v.z << ')';
    }
};
