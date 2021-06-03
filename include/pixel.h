#pragma once

struct Color
{
    float red;
    float green;
    float blue;

    Color operator*(const float &x) const { return Color{red * x, green * x, blue * x}; }
};

struct Pixel
{
    Color color;
};
