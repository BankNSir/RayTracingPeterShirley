#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera
{
    public:
        camera() 
        {
            origin = vec3(0.0, 0.0, 0.0);
            horizontal = vec3(4.0, 0.0, 0.0);
            vertical = vec3(0.0, 2.0, 0.0);
            lower_left_corner = vec3(-2.0, -1.0, -1.0);
        }

        // problem is hereeeeee
        camera(const vec3 &lookfrom,const vec3 &lookat,const vec3 &vup,float vfov, float aspect)
        {
            vec3 u, v, w;
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);
            origin = lookfrom;
            
            float theta = vfov * M_PI / 180;
            float half_height = tan(theta / 2);
            float half_width = aspect * half_height;
            lower_left_corner = origin - half_height*v - half_width*u - w;
            vertical = 2 * half_height * v;
            horizontal = 2 * half_width * u;
        }

        ray get_ray(float s, float t) const
        {
            return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
        }

        vec3 origin;
        vec3 horizontal;
        vec3 vertical;
        vec3 lower_left_corner;
};

#endif