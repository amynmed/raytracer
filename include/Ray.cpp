

#include "Ray.hpp"
#include "Vec3.hpp"



const Vec3& Ray::origin() const {return m_origin;}
const Vec3& Ray::direction() const {return m_direction;}

Ray::Ray(Vec3 position, Vec3 direction) : m_origin(position), m_direction(direction) {}

Vec3 Ray::position(double t) const
{
        return m_origin + m_direction * t;
}

