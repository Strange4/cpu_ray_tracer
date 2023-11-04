#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H
#include <memory>
#include <vector>

#include "hittable.h"

using std::shared_ptr;

class hittable_list : public hittable
{
public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) {}

    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    void clear() { objects.clear(); }

    bool hit(const ray &ray, interval &bounds,
             hit_record &h_record) const override
    {

        hit_record temp_hit;
        bool anything_hit = false;
        interval closest_hit(bounds.min, bounds.max);
        for (const shared_ptr<hittable> &object : objects)
        {
            if (object->hit(ray, closest_hit, temp_hit))
            {
                anything_hit = true;
                closest_hit.max = temp_hit.hit_distance;
                h_record = temp_hit;
            }
        }

        return anything_hit;
    }
};

#endif