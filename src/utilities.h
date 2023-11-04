#ifndef UTILITIES_H
#define UTILITIES_H

#include <cstdlib>
#include <limits>
#include <memory>
#include <random>

inline double hypothenuse(int width, int height)
{
    return sqrt(width * width + height * height);
}

const double infinity = std::numeric_limits<double>::infinity();

const double pi = 3.1415926535897932385;

inline double random_double()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator); // uniform distribution between 0 and 1
}

inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

#endif