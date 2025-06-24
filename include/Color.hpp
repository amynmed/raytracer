#pragma once

#include <cstdint>
#include "Vec3.hpp"
#include <iostream>

// 8888
struct RGBA
{
        uint8_t R, G, B, A;
};

struct RGB
{
        uint8_t R, G, B;
};



class Color
{
        public:

        uint8_t R() const {return m_rgba.R;}
        uint8_t G() const {return m_rgba.G;}
        uint8_t B() const {return m_rgba.B;}
        uint8_t A() const {return m_rgba.A;}

        void set_rgba(RGBA rgba) {m_rgba = rgba;}
        void set_rgb (RGB rgb)   {m_rgb  = rgb;}

        const Vec3& normal_rgb() const {return m_normal_rgb;}
        const RGBA& rgba() const {return m_rgba;}

        RGB rgb() const {return m_rgb;}

        void print_rgba() const { printf("RBGA: (%d, %d, %d, %d)\n", m_rgba.R, m_rgba.G, m_rgba.B, m_rgba.A); }
        void print_normal_rgb() const { printf("NORMAL_RGB: (%.2f, %.2f, %.2f)\n", m_normal_rgb.x(), m_normal_rgb.y(), m_normal_rgb.z()); }
        
         Color(Vec3);
         Color(double, double, double);
         //Color(uint8_t, uint8_t, uint8_t);
         //Color(RGB);
         Color() = default;
        ~Color() = default;

        


        private:
        RGBA m_rgba;
        RGB m_rgb;
        Vec3 m_normal_rgb;
        double alpha;


};

inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}