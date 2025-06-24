


#include <cmath>

#include "Vec3.hpp"



Vec3::Vec3() : m_x(0), m_y(0), m_z(0) {}

Vec3::Vec3(double x, double y, double z) : m_x(x), m_y(y), m_z(z) {}

double Vec3::x() const {return m_x;}
double Vec3::y() const {return m_y;}
double Vec3::z() const {return m_z;}


void Vec3::operator*=(double t)
{
        m_x *= t;
        m_y *= t;
        m_z *= t;
}

void Vec3::operator*=(const Vec3& v)
{
        m_x *= v.x();
        m_y *= v.y();
        m_z *= v.z();
}

void Vec3::operator+=(const Vec3& v)
{
        m_x += v.x();
        m_y += v.y();
        m_z += v.z();
}

Vec3 Vec3::operator+(const Vec3& v) const
{
        return Vec3(v.x() + m_x, v.y() + m_y, v.z() + m_z);
}

Vec3 Vec3::operator-(const Vec3& v) const
{
        return Vec3(m_x - v.x(), m_y - v.y(), m_z - v.z());
}

Vec3 Vec3::operator-() const
{
        return Vec3(-m_x, -m_y, -m_z);
}

Vec3 Vec3::operator*(double t) const
{
        return Vec3(m_x*t, m_y*t, m_z*t);
}

Vec3 Vec3::operator/(double t) const
{
        return *this * (1/t);
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

inline double Vec3::squared_magnitude() const
{
        return SQR(m_x) + SQR(m_y) + SQR(m_z);
}

inline double Vec3::magnitude() const
{
        return std::sqrt(squared_magnitude());
}

Vec3 VecUtils::normal(const Vec3& v)
{
        return v / v.magnitude();
}

/* inline Vec3 Vec3::normal(const Vec3& v) const
{
        double mag = magnitude();
        return v / mag;
} */