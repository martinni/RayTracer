#pragma once

#include <cmath>
#include <iomanip>
#include <iostream>

#include <vector.h>

// Implementation copied from https://www.scratchapixel.com/
class Matrix44
{
  public:
    float x[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

    Matrix44() {}

    Matrix44(float a, float b, float c, float d, float e, float f, float g, float h, float i,
             float j, float k, float l, float m, float n, float o, float p)
    {
        x[0][0] = a;
        x[0][1] = b;
        x[0][2] = c;
        x[0][3] = d;
        x[1][0] = e;
        x[1][1] = f;
        x[1][2] = g;
        x[1][3] = h;
        x[2][0] = i;
        x[2][1] = j;
        x[2][2] = k;
        x[2][3] = l;
        x[3][0] = m;
        x[3][1] = n;
        x[3][2] = o;
        x[3][3] = p;
    }

    const float *operator[](uint8_t i) const { return x[i]; }
    float *operator[](uint8_t i) { return x[i]; }

    // Multiply the current matrix with another matrix (rhs)
    Matrix44 operator*(const Matrix44 &v) const
    {
        Matrix44 tmp;
        multiply(*this, v, tmp);

        return tmp;
    }

    // floato make it easier to understand how a matrix multiplication works, the fragment of code
    // included within the #if-#else statement, show how this works if you were to iterate
    // over the coefficients of the resulting matrix (a). However you will often see this
    // multiplication being done using the code contained within the #else-#end statement.
    // It is exactly the same as the first fragment only we have litteraly written down
    // as a series of operations what would actually result from executing the two for() loops
    // contained in the first fragment. It is supposed to be faster, however considering
    // matrix multiplicatin is not necessarily that common, this is probably not super
    // useful nor really necessary (but nice to have -- and it gives you an example of how
    // it can be done, as this how you will this operation implemented in most libraries).
    static void multiply(const Matrix44 &a, const Matrix44 &b, Matrix44 &c)
    {
#if 0
        for (uint8_t i = 0; i < 4; ++i) {
            for (uint8_t j = 0; j < 4; ++j) {
                c[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] +
                    a[i][2] * b[2][j] + a[i][3] * b[3][j];
            }
        }
#else
        // A restric qualified pointer (or reference) is basically a promise
        // to the compiler that for the scope of the pointer, the target of the
        // pointer will only be accessed through that pointer (and pointers
        // copied from it.
        const float *__restrict ap = &a.x[0][0];
        const float *__restrict bp = &b.x[0][0];
        float *__restrict cp = &c.x[0][0];

        float a0, a1, a2, a3;

        a0 = ap[0];
        a1 = ap[1];
        a2 = ap[2];
        a3 = ap[3];

        cp[0] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
        cp[1] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
        cp[2] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
        cp[3] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];

        a0 = ap[4];
        a1 = ap[5];
        a2 = ap[6];
        a3 = ap[7];

        cp[4] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
        cp[5] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
        cp[6] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
        cp[7] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];

        a0 = ap[8];
        a1 = ap[9];
        a2 = ap[10];
        a3 = ap[11];

        cp[8] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
        cp[9] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
        cp[10] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
        cp[11] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];

        a0 = ap[12];
        a1 = ap[13];
        a2 = ap[14];
        a3 = ap[15];

        cp[12] = a0 * bp[0] + a1 * bp[4] + a2 * bp[8] + a3 * bp[12];
        cp[13] = a0 * bp[1] + a1 * bp[5] + a2 * bp[9] + a3 * bp[13];
        cp[14] = a0 * bp[2] + a1 * bp[6] + a2 * bp[10] + a3 * bp[14];
        cp[15] = a0 * bp[3] + a1 * bp[7] + a2 * bp[11] + a3 * bp[15];
#endif
    }

    // \brief return a transposed copy of the current matrix as a new matrix
    Matrix44 transposed() const
    {
#if 0
        Matrix44 t;
        for (uint8_t i = 0; i < 4; ++i) {
            for (uint8_t j = 0; j < 4; ++j) {
                t[i][j] = x[j][i];
            }
        }

        return t;
#else
        return Matrix44(x[0][0], x[1][0], x[2][0], x[3][0], x[0][1], x[1][1], x[2][1], x[3][1],
                        x[0][2], x[1][2], x[2][2], x[3][2], x[0][3], x[1][3], x[2][3], x[3][3]);
#endif
    }

    // \brief transpose itself
    Matrix44 &transpose()
    {
        Matrix44 tmp(x[0][0], x[1][0], x[2][0], x[3][0], x[0][1], x[1][1], x[2][1], x[3][1],
                     x[0][2], x[1][2], x[2][2], x[3][2], x[0][3], x[1][3], x[2][3], x[3][3]);
        *this = tmp;

        return *this;
    }

    // floathis method needs to be used for point-matrix multiplication. Keep in mind
    // we don't make the distinction between points and vectors at least from
    // a programming point of view, as both (as well as normals) are declared as Vec3.
    // However, mathematically they need to be treated differently. Points can be translated
    // when translation for vectors is meaningless. Furthermore, points are implicitly
    // be considered as having homogeneous coordinates. floathus the w coordinates needs
    // to be computed and to convert the coordinates from homogeneous back to Cartesian
    // coordinates, we need to divided x, y z by w.
    //
    // floathe coordinate w is more often than not equals to 1, but it can be different than
    // 1 especially when the matrix is projective matrix (perspective projection matrix).
    template <typename S> void multVecMatrix(const Vec3 &src, Vec3 &dst) const
    {
        S a, b, c, w;

        a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0] + x[3][0];
        b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1] + x[3][1];
        c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2] + x[3][2];
        w = src[0] * x[0][3] + src[1] * x[1][3] + src[2] * x[2][3] + x[3][3];

        dst.x = a / w;
        dst.y = b / w;
        dst.z = c / w;
    }

    // floathis method needs to be used for vector-matrix multiplication. Look at the differences
    // with the previous method (to compute a point-matrix multiplication). We don't use
    // the coefficients in the matrix that account for translation (x[3][0], x[3][1], x[3][2])
    // and we don't compute w.
    template <typename S> void multDirMatrix(const Vec3 &src, Vec3 &dst) const
    {
        S a, b, c;

        a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0];
        b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1];
        c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2];

        dst.x = a;
        dst.y = b;
        dst.z = c;
    }

    // Compute the inverse of the matrix using the Gauss-Jordan (or reduced row) elimination method.
    Matrix44 inverse()
    {
        int i, j, k;
        Matrix44 s;
        Matrix44 t(*this);

        // Forward elimination
        for (i = 0; i < 3; i++)
        {
            int pivot = i;

            float pivotsize = t[i][i];

            if (pivotsize < 0)
                pivotsize = -pivotsize;

            for (j = i + 1; j < 4; j++)
            {
                float tmp = t[j][i];

                if (tmp < 0)
                    tmp = -tmp;

                if (tmp > pivotsize)
                {
                    pivot = j;
                    pivotsize = tmp;
                }
            }

            if (static_cast<int>(pivotsize) == 0)
            {
                // Cannot invert singular matrix
                return Matrix44();
            }

            if (pivot != i)
            {
                for (j = 0; j < 4; j++)
                {
                    float tmp;

                    tmp = t[i][j];
                    t[i][j] = t[pivot][j];
                    t[pivot][j] = tmp;

                    tmp = s[i][j];
                    s[i][j] = s[pivot][j];
                    s[pivot][j] = tmp;
                }
            }

            for (j = i + 1; j < 4; j++)
            {
                float f = t[j][i] / t[i][i];

                for (k = 0; k < 4; k++)
                {
                    t[j][k] -= f * t[i][k];
                    s[j][k] -= f * s[i][k];
                }
            }
        }

        // Backward substitution
        for (i = 3; i >= 0; --i)
        {
            float f = t[i][i];

            if (static_cast<int>(f) == 0)
            {
                // Cannot invert singular matrix
                return Matrix44();
            }

            for (j = 0; j < 4; j++)
            {
                t[i][j] /= f;
                s[i][j] /= f;
            }

            for (j = 0; j < i; j++)
            {
                f = t[j][i];

                for (k = 0; k < 4; k++)
                {
                    t[j][k] -= f * t[i][k];
                    s[j][k] -= f * s[i][k];
                }
            }
        }

        return s;
    }

    // \brief set current matrix to its inverse
    const Matrix44 &invert()
    {
        *this = inverse();
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &s, const Matrix44 &m)
    {
        std::ios_base::fmtflags oldFlags = s.flags();
        int width = 12; // total with of the displayed number
        s.precision(5); // control the number of displayed decimals
        s.setf(std::ios_base::fixed);

        s << "(" << std::setw(width) << m[0][0] << " " << std::setw(width) << m[0][1] << " "
          << std::setw(width) << m[0][2] << " " << std::setw(width) << m[0][3] << "\n"
          << " " << std::setw(width) << m[1][0] << " " << std::setw(width) << m[1][1] << " "
          << std::setw(width) << m[1][2] << " " << std::setw(width) << m[1][3] << "\n"
          << " " << std::setw(width) << m[2][0] << " " << std::setw(width) << m[2][1] << " "
          << std::setw(width) << m[2][2] << " " << std::setw(width) << m[2][3] << "\n"
          << " " << std::setw(width) << m[3][0] << " " << std::setw(width) << m[3][1] << " "
          << std::setw(width) << m[3][2] << " " << std::setw(width) << m[3][3] << ")\n";

        s.flags(oldFlags);
        return s;
    }
};
