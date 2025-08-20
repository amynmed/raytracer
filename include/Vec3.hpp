#pragma once


#define SQR(x) (x * x)

#include <cmath>
#include <stdio.h>
#include "Utils.hpp"


class Vec3
{
public:
        Vec3();
        Vec3(double, double, double);

        double x() const;
        double y() const;
        double z() const;

        Vec3 operator-() const;
        Vec3 operator+(const Vec3&) const;
        Vec3 operator-(const Vec3&) const;
        Vec3 operator/(double t) const;
        Vec3 operator*(double t) const;

        void operator+=(const Vec3&);
        void operator*=(const Vec3&);
        void operator*=(double t);

        inline double squared_magnitude() const;
        inline double magnitude() const;
        inline Vec3 normal(const Vec3&) const;
        
        /* inline Vec3 random_unit_vector();
        inline Vec3 random_on_hemisphere(const Vec3&); */

        static Vec3 random() 
        {
                return Vec3(random_double(), random_double(), random_double());
        }

        static Vec3 random(double min, double max) 
        {
                return Vec3(random_double(min,max), random_double(min,max), random_double(min,max));
        }

        bool near_zero() const 
        {
                auto s = 1e-8;
                return (std::fabs(m_x) < s) &&
                       (std::fabs(m_y) < s) &&
                       (std::fabs(m_z) < s);
        }

        void print() {printf("VECTOR (x:%.2f, y:%.2f, z:%.2f)\n", x(), y(), z());}


private:
        double m_x, m_y, m_z;
};


namespace VecUtils
{
        Vec3 normal(const Vec3&);

        inline double dot(const Vec3& u, const Vec3& v) 
        {
                return u.x() * v.x()
                     + u.y() * v.y()
                     + u.z() * v.z();
        }

        inline Vec3 cross(const Vec3& u, const Vec3& v) 
        {
                return Vec3(u.y() * v.z() - u.z() * v.y(),
                        u.z() * v.x() - u.x() * v.z(),
                        u.x()* v.y() - u.y() * v.x());
        }

        inline Vec3 random_unit_vector() 
        {
                // Rejection method
                while (true) 
                {
                        auto p = Vec3::random(-1,1);
                        auto lensq = p.squared_magnitude();
                        // we use this statement to avoid getting big vectors when normalising
                        if (1e-160 < lensq && lensq <= 1)
                                return p / sqrt(lensq);
                        /* if (lensq <= 1)
                                return p / sqrt(lensq); */
                }
        }

        inline Vec3 random_on_hemisphere(const Vec3& normal) 
        {
                Vec3 on_unit_sphere = random_unit_vector();

                if (VecUtils::dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
                        return on_unit_sphere;
                else
                        return -on_unit_sphere;
        }

        inline Vec3 reflect(const Vec3& v, const Vec3& n)
        {
                return v - n*VecUtils::dot(v,n)*2;
        }

        inline Vec3 refract(const Vec3& uv, const Vec3& n, double eta_prime_ratio) 
        {
                auto cos_theta      = std::fmin(dot(-uv, n), 1.0);
                Vec3 r_out_perp     = (uv + n * cos_theta) * eta_prime_ratio;
                Vec3 r_out_parallel = n * (-std::sqrt(std::fabs(1.0 - r_out_perp.squared_magnitude()))) ;

                return r_out_perp + r_out_parallel;
        }


}; // namespace VecUtils
