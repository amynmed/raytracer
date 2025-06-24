#pragma once


#define SQR(x) (x * x)

#include <cmath>
#include <stdio.h>


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

}; // namespace VecUtils
