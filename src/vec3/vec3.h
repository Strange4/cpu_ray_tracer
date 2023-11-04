#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include "../utilities.h"
struct vec3
{
    double values[3];
    vec3() : values{0, 0, 0} {}
    vec3(double x, double y, double z) : values{x, y, z} {}

    double x() const { return values[0]; }
    double y() const { return values[1]; }
    double z() const { return values[2]; }

    static vec3 random()
    {
        return vec3(random_double(), random_double(), random_double());
    }
    static vec3 random(double min, double max)
    {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    bool near_zero() const;
    vec3 operator-() const;
    vec3 &operator+=(const vec3 &v);
    vec3 &operator*=(double t);
    vec3 &operator/=(double t);
    double length() const;
    double length_squared() const;
};

using point3 = vec3;

inline std::ostream &operator<<(std::ostream &out, const vec3 &v)
{
    return out << v.values[0] << ' ' << v.values[1] << ' ' << v.values[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v)
{
    return vec3(u.values[0] + v.values[0], u.values[1] + v.values[1],
                u.values[2] + v.values[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v)
{
    return vec3(u.values[0] - v.values[0], u.values[1] - v.values[1],
                u.values[2] - v.values[2]);
}

inline vec3 operator*(double c, const vec3 &u)
{
    return vec3(u.values[0] * c, u.values[1] * c, u.values[2] * c);
}

inline vec3 operator*(const vec3 &u, double c) { return c * u; }

inline vec3 operator/(const vec3 &u, double c) { return (1 / c) * u; }

inline double dot(const vec3 &u, const vec3 &v)
{
    return u.values[0] * v.values[0] + u.values[1] * v.values[1] +
           u.values[2] * v.values[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v)
{
    return vec3(u.values[1] * v.values[2] - u.values[2] * v.values[1],
                u.values[2] * v.values[0] - u.values[0] * v.values[2],
                u.values[0] * v.values[1] - u.values[1] * v.values[0]);
}

inline vec3 unit_vector(const vec3 &u) { return u / u.length(); }

inline vec3 random_vector_in_sphere()
{
    while (true)
    {
        vec3 v = vec3::random(-1, 1);
        if (v.length_squared() < 1)
            return v;
    }
}

inline vec3 random_unit_vector()
{
    return unit_vector(random_vector_in_sphere());
}

inline vec3 random_on_hemisphere(const vec3 &normal)
{
    vec3 unit_v = random_unit_vector();
    if (dot(unit_v, normal) > 0.0)
        return unit_v;
    else
        return -unit_v;
}

// reflects the vector with respect to the normal (bounces of the surface that is the normal)
inline vec3 reflect(const vec3 &in_vector, const vec3 &normal)
{
    return in_vector - 2 * dot(in_vector, normal) * normal;
}

inline double should_reflect_internally(double cos_theta, double refraction_ratio)
{
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    bool cannot_refract = (refraction_ratio * sin_theta) > 1.0;
    if (cannot_refract)
        return false;

    // shlick approximation
    double r0 = (1 - refraction_ratio) / (1 + refraction_ratio);
    r0 = r0 * r0;
    double shlick = r0 + (1 - r0) * pow((1 - cos_theta), 5);
    // honestly have no idea how this works
    return shlick > random_double();
}

inline vec3 refract(const vec3 &in_vector, const vec3 &normal, double refraction_ratio)
{
    double cos_theta = fmin(dot(-in_vector, normal), 1.0); // fmin because cos cannot be bigger than 1

    if (should_reflect_internally(cos_theta, refraction_ratio))
        return reflect(in_vector, normal);

    vec3 perpendicular_component = refraction_ratio * (in_vector + cos_theta * normal);
    vec3 parallel_component = -sqrt(fabs(1.0 - perpendicular_component.length_squared())) * normal;
    return perpendicular_component + parallel_component;
}

inline vec3 random_in_unit_disk()
{
    while (true)
    {
        vec3 p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

#endif