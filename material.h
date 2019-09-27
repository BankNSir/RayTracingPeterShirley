#ifndef MATERIALH
#define MATERIALH

#include "hitable.h"
#include "random.h"

vec3 random_in_unit_sphere()
{
    vec3 s;
    do
    {
        s = vec3(random(), random(), random()); 
    }
    while (s.length() > 1);

    return s;
}

vec3 reflected (const vec3 &ray_in,const vec3 &normal)
{
    vec3 B = dot(ray_in, normal) * normal;
    return ray_in - 2 * B;
}

class material
{
    public:
        virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const = 0;
};

class lambertian : public material
{
    public:
        lambertian(const vec3 &a) : albedo(a) {}
        virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
        {
            scattered = ray(rec.p, rec.normal + random_in_unit_sphere());
            attenuation = albedo;
            return true;
        }

        vec3 albedo;
};

class metal : public material
{
    public:
        metal(const vec3 &a) : albedo(a) {}
        virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
        {
            scattered = ray(rec.p, reflected(r_in.direction(), rec.normal));
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
    
        vec3 albedo;
};


#endif