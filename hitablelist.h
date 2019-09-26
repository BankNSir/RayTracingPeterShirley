#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.h"

class hitable_list : public hitable
{
    public:
        hitable **list;
        int list_size;
        hitable_list() {}
        hitable_list(hitable **l, int n) : list_size(n) ,list(l) {}
        virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
};

bool hitable_list::hit(const ray &r, float t_min, float t_max, hit_record &rec) const
{
    bool hit_anything = false;
    float closest_so_far = t_max;
    hit_record temp_rec;
    // for each object check hit
    for (int i = 0; i < list_size; i++)
    {
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif