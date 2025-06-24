#pragma once


#include "Vec3.hpp"
#include "Ray.hpp"

// intersection info
class Intersection
{
        public:
        Vec3 p;
        Vec3 normal;
        double t;
        bool front_face;


        void set_face_normal(const Ray& r, const Vec3& outward_normal) 
        {

                front_face = VecUtils::dot(r.direction(), outward_normal) < 0;
                normal     = front_face ? outward_normal : -outward_normal;
        }
};

class Object
{
        public:
        Vec3 get_position();
        void set_position(Vec3);
        void scale(float);

        

        Object(Vec3);
        Object(Vec3, Vec3);
        Object() = default;
        ~Object() = default;

        protected:
        Vec3 m_position;
        Vec3 m_size;
};