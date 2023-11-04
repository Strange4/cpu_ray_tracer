#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"

class hit_record;

class material
{
public:
    virtual ~material() = default;
    // returns true or false depending if the ray died out in between the cracks of the material
    virtual bool scatter(const ray &r_in, const hit_record &hit, color &coloring, ray &r_out) const = 0;
};

class LambertialReflection : public material
{
public:
    LambertialReflection(const color &color_tint) : albedo(color_tint) {}
    bool scatter(const ray &r_in, const hit_record &hit, color &coloring, ray &r_out) const override
    {
        point3 point_outside_sphere = hit.normal + random_unit_vector();
        // if the point is oposite to the normal, use the normal because it causes problems
        if (point_outside_sphere.near_zero())
            point_outside_sphere = hit.normal;
        r_out = ray(hit.p, point_outside_sphere);
        coloring = albedo;
        return true;
    }

private:
    color albedo;
};

class MetalReflection : public material
{

public:
    MetalReflection(const color &color_tint) : albedo(color_tint), roughness(0) {}
    MetalReflection(const color &color_tint, double _roughness) : albedo(color_tint), roughness(_roughness) {}

    bool scatter(const ray &r_in, const hit_record &hit, color &coloring, ray &r_out) const override
    {
        // todo: verify that this doesn't break anything by the direction not being a unit vector
        vec3 reflected_direction = reflect(r_in.direction(), hit.normal) + roughness * random_unit_vector();
        if (dot(reflected_direction, hit.normal) <= 0)
            return false;

        r_out = ray(hit.p, reflected_direction);
        coloring = albedo;
        return true;
    }

private:
    color albedo;
    double roughness;
};

// kind of like glass or water.
class DielectricMaterial : public material
{
public:
    DielectricMaterial(double index_of_refraction) : ir(index_of_refraction) {}
    bool scatter(const ray &r_in, const hit_record &hit, color &coloring, ray &r_out) const override
    {
        coloring = color(1.0, 1.0, 1.0);
        double refraction_ratio = hit.is_front_face ? (1.0 / ir) : ir;
        vec3 ray_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-ray_direction, hit.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        vec3 out_ray_direction = refract(ray_direction, hit.normal, refraction_ratio);
        r_out = ray(hit.p, out_ray_direction);
        return true;
    }

private:
    double ir;
};

#endif