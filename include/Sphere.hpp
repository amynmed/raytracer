#pragma once

#include "Interval.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

class Sphere : public Mesh
{

public:
	Sphere(Vec3 center, double radius, std::shared_ptr<Material> mat)
	    : Mesh(center, mat), m_radius(std::fmax(0, radius))
	{}
	bool hit(const Ray& r, Interval, Intersection& intersection_info) const override;

private:
	double m_radius;
};