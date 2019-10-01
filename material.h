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

vec3 reflect(const vec3 &v_in,const vec3 &normal)
{
    vec3 B = dot(v_in, normal) * normal;
    return v_in - 2 * B;
}

bool refract(const vec3 &v, float ni_over_nt, const vec3 &n, vec3 &refracted)
{
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1 - dt*dt);

    if (discriminant > 0)  // verify if the ray get refracted
    {
        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
        return true;
    }
    else
    {
        return false;
    }
}

float schlick(float cosine, float ref_idx)
{
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0)*pow(1 - cosine, 5);
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
            scattered = ray(rec.p, rec.normal + random_in_unit_sphere(), r_in._time);
            attenuation = albedo;
            return true;
        }

        vec3 albedo;
};

class metal : public material
{
    public:
        metal(const vec3 &a) : albedo(a) { fuzz = 0; }
        metal(const vec3 &a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
        virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
        {
            scattered = ray(rec.p, reflect(r_in.direction(), rec.normal) + fuzz * random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
    
        vec3 albedo;
        float fuzz;
};

class dielectric : public material
{
    public:
        dielectric(float n) : ref_idx(n) { albedo = vec3(1, 1, 1); }

        // overload constructor here for color dielectric
        virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
        {
            attenuation = albedo;
            vec3 refracted;
            vec3 outward_normal;
            vec3 reflected = reflect(r_in.direction(), outward_normal);
            float ni_over_nt; // for setting inversion if ray are coming out of object
            float cosine;
            float reflect_prob;

            // time to set inverse ray
            if (dot(r_in.direction(), rec.normal) > 0)  // insideout
            {
                ni_over_nt = ref_idx;
                outward_normal = -1 * rec.normal;
                cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
            }
            else  // outsidein
            {
                ni_over_nt = 1 / ref_idx;
                outward_normal = rec.normal;
                cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
            }
            
            // full refracted ray direction expression : (ni/nt)*(vec_in - n * dot(vec_in, n)) - n*sqrt(discriminant)
            // check refract or reflect 
            if (refract(r_in.direction(), ni_over_nt, outward_normal, refracted))
            {
                reflect_prob = schlick(cosine, ref_idx);
            }
            else
            {
                reflect_prob = 1.0;
            }

            // random reflect vs refrac
            if (random() > reflect_prob) // refrac
            {
                scattered = ray(rec.p, refracted);
            }
            else // reflect
            {
                scattered = ray(rec.p, reflected);
            }
            
            return true;
        }

        vec3 albedo;
        float ref_idx;

};

#endif