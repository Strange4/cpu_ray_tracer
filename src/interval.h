#ifndef INTERVAL_H
#define INTERVAL_H

#include "utilities.h"

struct interval
{
    double min, max;

    interval() : min(+infinity), max(-infinity) {}

    interval(double _min, double _max) : min(_min), max(_max) {}

    bool contains(double x) const { return min <= x && x <= max; }

    bool contains_exclusive(double x) const { return min < x && x < max; }

    double clamp(double x) const
    {
        if (x > max)
            return max;
        if (x < min)
            return min;
        return x;
    }
};

const interval emtpy = interval(+infinity, -infinity);
const interval universe = interval(-infinity, +infinity);
#endif