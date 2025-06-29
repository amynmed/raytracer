#pragma once


#include "Material.hpp"
#include "Color.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"


class Lambertian: public Material
{

        public:
        Lambertian(const Color& albedo) : m_albedo(albedo) {}
        Lambertian() = default;
        ~Lambertian() = default;

        bool scatter(const Ray& r_in, Intersection& intersection, Color& attenuation, Ray& scattered) const override 
        {
                auto scatter_direction = intersection.normal + VecUtils::random_unit_vector();

                if (scatter_direction.near_zero())
                        scatter_direction = intersection.normal;

                scattered = Ray(intersection.p, scatter_direction);
                attenuation = m_albedo;
                return true;
        }

        private:
        Color m_albedo;
};