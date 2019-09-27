#include <iostream>
#include <fstream>
#include <cstdlib>
#include "hitable_list.h"
#include "sphere.h"
#include "float.h"
#include "camera.h"
#include "random.h"

using namespace std;

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

// function to lerp color for each pixel
vec3 color(const ray &r, hitable *world)
{
    hit_record rec;
    if (world->hit(r, 0.001, FLT_MAX, rec))
    {
        // vec3 n = rec.normal / 2 + vec3(0.5, 0.5, 0.5);
        ray diffused_ray(rec.p, rec.normal + random_in_unit_sphere());
        return 0.5 * color(diffused_ray, world);
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

    hitable *list[2];
    list[0] = new sphere(vec3(0.0, 0.0, -1.0), 0.5);
    list[1] = new sphere(vec3(0.0, -100.5, -1.0), 100);
    hitable *world = new hitable_list(list, 2);

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
                col += color(r, world);
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

    delete(list[0]);
    delete(list[1]);
    file.close();
}