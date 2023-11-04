#ifndef HITTABLE_H
#define HITTABLE_H

#include "interval.h"
#include "common.h"

class material;

struct hit_record
{
    point3 p; // the point where it is hit
    vec3 normal;
    double hit_distance;
    shared_ptr<material> mat;
    bool is_front_face;
    void set_face_normal(const ray &ray, const vec3 &outward_normal)
    {
        // the outward normal must have a unit length. this is done outside
        is_front_face = dot(ray.direction(), outward_normal) < 0;
        normal = is_front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
    virtual ~hittable() = default;

    virtual bool hit(const ray &ray, interval &bounds,
                     hit_record &h_record) const = 0;
};

#endif