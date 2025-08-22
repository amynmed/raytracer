

#include <cmath>

#include "Vec3.hpp"

Vec3::Vec3() : m_x(0), m_y(0), m_z(0) {}

Vec3::Vec3(double x, double y, double z) : m_x(x), m_y(y), m_z(z) {}

double Vec3::x() const
{
	return m_x;
}
double Vec3::y() const
{
	return m_y;
}
double Vec3::z() const
{
	return m_z;
}

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
	return Vec3(m_x * t, m_y * t, m_z * t);
}

Vec3 Vec3::operator/(double t) const
{
	return *this * (1 / t);
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