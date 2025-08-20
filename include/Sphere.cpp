
#include "Sphere.hpp"


// * FIND A WAY TO SIMD THIS
// MAYBE CREATE A FUNCTION THAT TAKES A VECTOR OF RAYS (PACKET) THEN USE SIMD OPS
// TO CALCULATE INTERSECTIONS ?
bool Sphere::hit(const Ray& r, Interval interval, Intersection& intersection_info) const
{
        Vec3 oc = m_position - r.origin();

        auto a = VecUtils::dot(r.direction(), r.direction());
        auto h = VecUtils::dot(r.direction(), oc);
        auto c = VecUtils::dot(oc, oc) - m_radius*m_radius;


        auto discriminant = h*h - a*c;


        if (discriminant < 0) 
                return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        
        if (!interval.surrounds(root)) 
        {
                root = (h + sqrtd) / a;

                if (!interval.surrounds(root))
                        return false;
        }

        intersection_info.t      = root;
        intersection_info.p      = r.position(intersection_info.t);
        intersection_info.normal = (intersection_info.p - m_position) / m_radius;

        Vec3 outward_normal = (intersection_info.p - m_position) / m_radius;
        intersection_info.set_face_normal(r, outward_normal);

        intersection_info.mat = m_material;

        return true;
}