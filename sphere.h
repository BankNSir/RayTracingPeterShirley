#ifndef SPHEREH
#define SPHEREH

#include "material.h"

class sphere : public hitable
{
    public:
        sphere() {}
        sphere(const vec3 &cen, float r, material* mat) : center(cen), radius(r) , mat_ptr(mat) {};
        virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;      

        vec3 center;
        float radius;
        material* mat_ptr;
};

bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const
{
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(r.direction(), oc);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0)
    {
        float temp = (-b - sqrt(discriminant)) / a;
        if (t_min < temp && temp < t_max)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - this->center) / radius;
            rec.mat_ptr = this->mat_ptr;
            return true;
        }
    
        temp = (-b + sqrt(discriminant)) / a;
        if (t_min < temp && temp < t_max)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - this->center) / radius;
            rec.mat_ptr = this->mat_ptr;
            return true;
        }
    }

    return false;
}

class moving_sphere : public hitable
{
    public:
        moving_sphere() {}
        moving_sphere(const vec3 &cen0, const vec3 &cen1, float t0, float t1, float r, material *mat) : 
            center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r) , mat_ptr(mat) {};
        virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;      

        vec3 center (float ti) const
        {
            return center0 + (ti-time0) / (time1-time0) * (center1 - center0);
        }

        vec3 center0;
        vec3 center1;
        float time0, time1;
        float radius;
        material* mat_ptr;
};

bool moving_sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const
{
    vec3 oc = r.origin() - center(r._time);
    float a = dot(r.direction(), r.direction());
    float b = dot(r.direction(), oc);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0)
    {
        float temp = (-b - sqrt(discriminant)) / a;
        if (t_min < temp && temp < t_max)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - this->center(r._time)) / radius;
            rec.mat_ptr = this->mat_ptr;
            return true;
        }
    
        temp = (-b + sqrt(discriminant)) / a;
        if (t_min < temp && temp < t_max)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - this->center(r._time)) / radius;
            rec.mat_ptr = this->mat_ptr;
            return true;
        }
    }

    return false;
}

#endif