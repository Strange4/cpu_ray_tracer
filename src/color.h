#ifndef COLOR_H
#define COLOR_H

#include "png.hpp"
#include "interval.h"
#include "iostream"

struct color
{
    double values[3];
    color() : values{0, 0, 0} {}
    color(double r, double g, double b) : values{r, g, b} {}

    double r() const { return values[0]; }
    double g() const { return values[1]; }
    double b() const { return values[2]; }

    explicit operator png::rgb_pixel() const { return png::rgb_pixel(sqrt(values[0]) * 255, sqrt(values[1]) * 255, sqrt(values[2]) * 255); }

    color &operator+=(const color &v)
    {
        values[0] += v.r();
        values[1] += v.g();
        values[2] += v.b();
        return *this;
    }

    static color random()
    {
        return color(random_double(), random_double(), random_double());
    }

    static color random(double min, double max)
    {
        return color(random_double(min, max), random_double(min, max), random_double(min, max));
    }
};

inline color operator+(const color &c0, const color &c1)
{
    return color(c0.values[0] + c1.values[0], c0.values[1] + c1.values[1],
                 c0.values[2] + c1.values[2]);
}

inline color operator+(const color &c, double a)
{
    return color(c.values[0] + a, c.values[1] + a, c.values[2] + a);
}

inline color operator+(double a, const color &c) { return c + a; }

inline color operator-(const color &c0, const color &c1)
{
    return color(c0.values[0] - c1.values[0], c0.values[1] - c1.values[1],
                 c0.values[2] - c1.values[2]);
}

inline color operator*(double c, const color &u)
{
    return color(u.values[0] * c, u.values[1] * c, u.values[2] * c);
}

inline color operator*(const color &c0, const color &c1)
{
    return color(c0.values[0] * c1.values[0], c0.values[1] * c1.values[1], c0.values[2] * c1.values[2]);
}

inline color operator*(const color &u, double c) { return c * u; }

inline color operator/(const color &u, double c)
{
    return color(u.values[0] / c, u.values[1] / c, u.values[2] / c);
}

inline void write_color(int x, int y, png::image<png::rgb_pixel> &image, color pixel_color)
{
    static const interval color_interval(0, 1);
    pixel_color.values[0] = color_interval.clamp(pixel_color.r());
    pixel_color.values[1] = color_interval.clamp(pixel_color.g());
    pixel_color.values[2] = color_interval.clamp(pixel_color.b());
    image.set_pixel(x, y, static_cast<png::rgb_pixel>(pixel_color));
}

#endif