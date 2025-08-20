#pragma once

#include "Object.hpp"
#include "Material.hpp"
#include "Interval.hpp"

class Mesh: public Object
{
public:
        Mesh(Vec3 position, std::shared_ptr<Material> material): Object(position), m_material(material){}
        Mesh(Vec3 position): Object(position){}
        Mesh() = default;
        ~Mesh() = default;
        
        virtual bool hit(const Ray& r, Interval, Intersection& intersection_info) const = 0;

protected:
        std::shared_ptr<Material> m_material;
};