#pragma once

#include "Color.hpp"
#include "Ray.hpp"
#include "Object.hpp"

enum class MATERIAL_TYPE
{
        PHONG,
        BLINN,
        LAMBERT,
        METAL
};

class Material
{
        public:
        void set_color(Color);

        Material() = default;
        virtual ~Material() = default;
        virtual bool scatter(const Ray& r_in, Intersection& intersection, Color& attenuation, Ray& scattered) const {return false;}


        private:
        MATERIAL_TYPE type;
        Color color;
};

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
        float m_roughness;
};