#pragma once


#include "Material.hpp"
#include "Color.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"


class Metal: public Material
{

        public:
        Metal(const Color& albedo) : m_albedo(albedo) {}
        Metal() = default;
        ~Metal() = default;

        bool scatter(const Ray& r_in, Intersection& intersection, Color& attenuation, Ray& scattered) const override 
        {
                Vec3 reflected = VecUtils::reflect(r_in.direction(), intersection.normal);

                scattered   = Ray(intersection.p, reflected);
                attenuation = m_albedo;

                return true;
        }

        private:
        Color m_albedo;
};