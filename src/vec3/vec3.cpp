#include "vec3.h"
#include "../utilities.h"

bool vec3::near_zero() const
{
    double small_number = 1e-8;
    return (fabs(values[0]) < small_number) && (fabs(values[1]) < small_number) && (fabs(values[2]) < small_number);
}

vec3 vec3::operator-() const { return vec3(-values[0], -values[1], -values[2]); }

vec3 &vec3::operator+=(const vec3 &v)
{
    values[0] += v.x();
    values[1] += v.y();
    values[2] += v.z();
    return *this;
}

vec3 &vec3::operator*=(double t)
{
    values[0] *= t;
    values[1] *= t;
    values[2] *= t;
    return *this;
}

vec3 &vec3::operator/=(double t) { return *this *= 1 / t; }

double vec3::length() const { return sqrt(length_squared()); }

double vec3::length_squared() const
{
    return values[0] * values[0] + values[1] * values[1] +
           values[2] * values[2];
}
