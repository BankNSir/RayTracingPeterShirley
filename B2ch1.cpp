#include <iostream>
#include <fstream>
#include <cstdlib>
#include "hitable_list.h"
#include "sphere.h"
#include "float.h"
#include "camera.h"

using namespace std;

// function to return color for each pixel
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
        // object absorb all r g b colors
        return vec3(0.0, 0.0, 0.0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    // if y = 1 want t = 1 if y = -1 want t = 0
    float t = 0.5 * (unit_direction.y()) + 0.5;
    return t * vec3(0.5, 0.7, 1.0) + (1.0-t) * vec3(1.0, 1.0, 1.0);
}

// function for

hitable *random_scene(hitable **listOfObject, int numberOfObject)
{
    int count = 1;
    for (int i = -11; i < 11; i++)
    {
        for (int j = -11; j < 11; j++)
        {
            vec3 center(i + 0.9*random(), 0.2, j + 0.9*random());
            
            if ((center - vec3(6, 1, -2)).length() > 1)
            {    
                // random material
                float material_selector = random();

                if (material_selector < 0.6) // lambertian
                {
                    // listOfObject[count++] = new sphere(center, 0.3, 
                    // new lambertian(vec3(random()*random(), random()*random(), random()*random())));
                    vec3 center2 = center + 0.5 * vec3(0, random(), 0); // random moving in vertical
                    listOfObject[count++] = new moving_sphere(center, center2, 0, 1, 0.2, 
                    new lambertian(vec3(random()*random(), random()*random(), random()*random())));
                }
                else if (material_selector < 0.8) // matal
                {
                    listOfObject[count++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1 + random()), 0.5 * (1 + random()), 0.5 * (1 + random()))));
                }
                else // dielectric
                {
                    listOfObject[count++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }   
        }    
    }

    listOfObject[count++] = new sphere(vec3(-4, 1, 0), 1, new lambertian(vec3(0.4, 0.2, 0.1)));
    listOfObject[count++] = new sphere(vec3(4, 1, 0), 1, new metal(vec3(0.7, 0.6, 0.5)));
    listOfObject[count++] = new sphere(vec3(0, 1, 0), 1, new dielectric(1.5));

    return new hitable_list(listOfObject, count);
}

int main ()
{
    fstream file;
    file.open("./All Picture/Book2Ch1.PPM", ios::out | ios::trunc);

    int nx = 600;
    int ny = 300;
    int ns = 80;
    
    std::cout << "P3" << "\n" << nx << " " << ny << "\n255\n";
    file << "P3" << "\n" << nx << " " << ny << "\n255\n";

    // camera setup
    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    float aperture = 0;
    float focus_dist = (lookfrom - lookat).length();
    // float shutter_time = 1.0;
    camera cam(lookfrom, lookat, vec3(0, 1, 0), 45, float(nx)/float(ny), aperture, focus_dist, 0.0, 0.1);
    // camera cam;

    // environment
    // hitable *list[4];
    // list[0] = new sphere(vec3(0.0, 0.0, -1.0), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    // list[1] = new sphere(vec3(0.0, -100.5, -1.0), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    // list[2] = new sphere(vec3(1.0, 0.0, -1.0), 0.5, new metal(vec3(0.8, 0.6, 0.2)));
    // list[3] = new sphere(vec3(-1.0, 0.0, -1.0), 0.5, new dielectric(1.5));
    // hitable *world = new hitable_list(list, 4);
    int numberOfObject = 500;
    hitable *list[numberOfObject];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    hitable *world = random_scene(list, numberOfObject);
    
    // path tracing
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