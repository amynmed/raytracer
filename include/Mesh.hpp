#pragma once

#include "Object.hpp"
#include "Material.hpp"
#include "Interval.hpp"

class Mesh: public Object
{
        public:
        Mesh(Vec3 position): Object(position){}
        Mesh() = default;
        ~Mesh() = default;
        
        virtual bool hit(const Ray& r, Interval, Intersection& intersection_info) const = 0;

        private:
        Material material;
};