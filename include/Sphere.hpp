#pragma once


#include "Mesh.hpp"
#include "Interval.hpp"



class Sphere: public Mesh
{

        public:
        Sphere(Vec3 center, double radius) : Mesh(center), m_radius(std::fmax(0,radius)) {}
        bool hit(const Ray& r, Interval, Intersection& intersection_info) const override;

        private:
        double m_radius;

};