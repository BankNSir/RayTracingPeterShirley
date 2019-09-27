#ifndef RANDOMH
#define RANDOMH

#include <random>

float random(float a, float b)
{
    return a + (float) rand() / (float) (RAND_MAX + 1) * (b - a);
}

float random()
{
    return (float) rand() / (float) (RAND_MAX + 1);
}

#endif