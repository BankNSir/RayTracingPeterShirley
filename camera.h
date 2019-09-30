#ifndef CAMERAH
#define CAMERAH

#include "ray.h"
#include "random.h"

vec3 random_in_unit_circle()
{
    vec3 temp;
    do
    {
        temp = vec3(random(), random(), 0.0)*2 - vec3(1, 1, 0);
    } 
    while (dot(temp, temp) > 1);

    return temp;
}

class camera
{
    public:
        camera() 
        {
            origin = vec3(0.0, 0.0, 0.0);
            horizontal = vec3(4.0, 0.0, 0.0);
            vertical = vec3(0.0, 2.0, 0.0);
            lower_left_corner = vec3(-2.0, -1.0, -1.0);
            lense_radius = 0;
        }

        camera(const vec3 &lookfrom,const vec3 &lookat,const vec3 &vup,float vfov, float aspect, float aperture, float focus_dist)
        {            
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);
            
            origin = lookfrom;
            lense_radius = aperture / 2;
            
            // can be optimize
            float theta = vfov * M_PI / 180;
            float half_height = tan(theta / 2);
            float half_width = aspect * half_height;
            vertical = 2 * focus_dist * half_height * v;
            horizontal = 2 * focus_dist * half_width * u;
            lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;
            // lower_left_corner = origin - ((half_height*v + half_width*u + w) * focus_dist);
            // vertical = 2 * focus_dist * half_height * v;
            // horizontal = 2 * focus_dist * half_width * u;
        }

        ray get_ray(float s, float t) const
        {
            // random_2D.x() * u + random_2D.y() * v
            vec3 random_direction = lense_radius * random_in_unit_circle();
            random_direction = random_direction.x() * u + random_direction.y() * v;
            return ray(origin + random_direction, lower_left_corner + s * horizontal + t * vertical - origin - random_direction);
        }

        vec3 origin;
        vec3 horizontal;
        vec3 vertical;
        vec3 lower_left_corner;
        vec3 u, v, w;
        float lense_radius;
};

#endif