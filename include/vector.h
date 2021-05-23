#pragma once

#include <cmath>
#include <iostream>

// Implementation copied from https://www.scratchapixel.com/
template <typename T> class Vec3
{
  public:
    T x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(T xx) : x(xx), y(xx), z(xx) {}
    Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}

    Vec3 operator+(const Vec3 &v) const { return Vec3(x + v.x, y + v.y, z + v.z); }

    Vec3 operator-(const Vec3 &v) const { return Vec3(x - v.x, y - v.y, z - v.z); }

    Vec3 operator*(const T &r) const { return Vec3(x * r, y * r, z * r); }

    T dotProduct(const Vec3<T> &v) const { return x * v.x + y * v.y + z * v.z; }

    T crossProduct(const Vec3<T> &v) const
    {
        return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    T norm() const { return x * x + y * y + z * z; }

    T length() const { return sqrt(norm()); }

    // The next two operators are sometimes called access operators or
    // accessors. The Vec coordinates can be accessed that way v[0], v[1], v[2],
    // rather than using the more traditional form v.x, v.y, v.z. This useful
    // when vectors are used in loops: the coordinates can be accessed with the
    // loop index (e.g. v[i]).
    //[/comment]
    const T &operator[](uint8_t i) const { return (&x)[i]; }
    T &operator[](uint8_t i) { return (&x)[i]; }

    Vec3 &normalize()
    {
        T n = norm();
        if (n > 0)
        {
            T factor = 1 / sqrt(n);
            x *= factor, y *= factor, z *= factor;
        }

        return *this;
    }

    friend std::ostream &operator<<(std::ostream &s, const Vec3<T> &v)
    {
        return s << '(' << v.x << ' ' << v.y << ' ' << v.z << ')';
    }
};

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
