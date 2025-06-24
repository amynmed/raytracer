#pragma once

#include <random>

const double PI = 3.1415926535897932385;

inline double deg_to_rad(double deg)
{
        return deg * PI / 180.;
}

inline double random_double() 
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    
    return distribution(generator);
}