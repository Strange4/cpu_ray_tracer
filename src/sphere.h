#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "utilities.h"
#include <cmath>

class sphere : public hittable
{
public:
    point3 center;
    double radius;
    shared_ptr<material> mat;
    sphere(double r, const point3 &c, shared_ptr<material> _material) : center(c), radius(r), mat(_material) {}

    bool hit(const ray &ray, interval &bounds,
             hit_record &hit_data) const override
    {
        // find the variables for the sphere intersection formula
        vec3 origin_center_v = ray.origin() - center;
        double a = ray.direction().length_squared();
        double half_b = dot(origin_center_v, ray.direction());
        double c = origin_center_v.length_squared() - (radius * radius);
        double discriminant = half_b * half_b - a * c;

        // if the discriminant is < 0, then there is no solution (doesn't hit)
        if (discriminant < 0)
            return false;

        double rooted = sqrt(discriminant);
        double hit_distance = (-half_b - rooted) / a;

        // check if out of bounds
        if (!bounds.contains_exclusive(hit_distance))
        {
            // if the closest one is out of bounds then try the other one
            hit_distance = (-half_b + rooted) / a;
            if (!bounds.contains_exclusive(hit_distance))
                return false;
        }

        hit_data.hit_distance = hit_distance;
        hit_data.p = ray.at(hit_distance);
        // we divide by the radius so that the normal is a unit vector
        vec3 outward_normal = (hit_data.p - center) / radius;
        hit_data.set_face_normal(ray, outward_normal);
        hit_data.mat = mat;
        return true;
    }
};

#endif