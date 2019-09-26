#include<iostream>
#include<fstream>
#include "ray.h"
using namespace std;

// function to verified if ray intersect a circle
bool hit_sphere(const vec3& center, float radius,const ray &r)
{
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2 * dot(r.direction(), oc);
    float c = dot(oc, oc) - radius * radius;
    return (b*b - 4*a*c >= 0);
}

// function to lerp color for each pixel
vec3 color(const ray &r)
{
    // draw circle
    // vec3 center(0.0, 0.0, -1.0);
    if (hit_sphere(vec3(0.0, 0.0, -1.0), 0.5, r))
    {
        return vec3(1.0, 0.0, 0.0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    // if y = 1 want t = 1 if y = -1 want t = 0
    float t = 0.5 * (unit_direction.y()) + 0.5;
    return t * vec3(0.5, 0.7, 1.0) + (1.0-t) * vec3(1.0, 1.0, 1.0);
}


int main ()
{
    fstream file;
    file.open("./All Picture/Ch4.PPM", ios::out | ios::trunc);
    int nx = 200;
    int ny = 100;

    std::cout << "P3" << "\n" << nx << " " << ny << "\n255\n";
    file << "P3" << "\n" << nx << " " << ny << "\n255\n";

    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            float scaled_pixel_x = float(i) / float(nx);
            float scaled_pixel_y = float(j) / float(ny);
            vec3 direction = lower_left_corner + horizontal*scaled_pixel_x + vertical*scaled_pixel_y;
            ray r(origin, direction);
            vec3 col = color(r);
            
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            std::cout << ir << " " << ig << " " << ib << "\n";
            file << ir << " " << ig << " " << ib << "\n";
        }
    }

    file.close();
}
