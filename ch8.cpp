#include <iostream>
#include <fstream>
#include <cstdlib>
#include "hitable_list.h"
#include "sphere.h"
#include "float.h"
#include "camera.h"
#include "random.h"
#include "material.h"

using namespace std;

// function to lerp color for each pixel
vec3 color(const ray &r, hitable *world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001, FLT_MAX, rec))
    {
        vec3 albedo;
        ray scattered;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, albedo, scattered))
        {
            return albedo * color(scattered, world, depth + 1);
        }
        
        return vec3(0.0, 0.0, 0.0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    // if y = 1 want t = 1 if y = -1 want t = 0
    float t = 0.5 * (unit_direction.y()) + 0.5;
    return t * vec3(0.5, 0.7, 1.0) + (1.0-t) * vec3(1.0, 1.0, 1.0);
}


int main ()
{
    fstream file;
    file.open("./All Picture/Ch7.PPM", ios::out | ios::trunc);

    int nx = 200;
    int ny = 100;
    int ns = 100;
    
    std::cout << "P3" << "\n" << nx << " " << ny << "\n255\n";
    file << "P3" << "\n" << nx << " " << ny << "\n255\n";

    camera cam;

    hitable *list[4];
    list[0] = new sphere(vec3(0.0, 0.0, -1.0), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
    list[1] = new sphere(vec3(0.0, -100.5, -1.0), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1.0, 0.0, -1.0), 0.5, new metal(vec3(0.8, 0.6, 0.2)));
    list[3] = new sphere(vec3(-1.0, 0.0, -1.0), 0.5, new metal(vec3(0.8, 0.6, 0.4)));
    hitable *world = new hitable_list(list, 4);

    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            vec3 col(0.0, 0.0, 0.0);
            for (int s = 0; s < ns; s++)
            {
                float u = (float(i) + random()) / float(nx);
                float v = (float(j) + random()) / float(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }
            
            col /= float(ns);
            
            // add gamma = 2
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            std::cout << ir << " " << ig << " " << ib << "\n";
            file << ir << " " << ig << " " << ib << "\n";
        }
    }

    // delete(list[0]);
    // delete(list[1]);
    file.close();
}