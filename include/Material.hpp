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


//
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


//
class Lambertian: public Material
{
public:
        Lambertian(const Color& albedo) : m_albedo(albedo) {}
        Lambertian()  = default;
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


//
class Metal: public Material
{
public:

        Metal(const Color& albedo, double roughness) : m_albedo(albedo), m_roughness(roughness < 1 ? roughness : 1) {}
        Metal(const Color& albedo) : m_albedo(albedo) {}
        Metal()  = default;
        ~Metal() = default;

        bool scatter(const Ray& r_in, Intersection& intersection, Color& attenuation, Ray& scattered) const override 
        {
                Vec3 reflected = VecUtils::reflect(r_in.direction(), intersection.normal);
                reflected = VecUtils::normal(reflected) + VecUtils::random_unit_vector() * m_roughness;
                scattered   = Ray(intersection.p, reflected);
                attenuation = m_albedo;


                 return (VecUtils::dot(scattered.direction(), intersection.normal) > 0);

                return true;
        }

private:
        Color m_albedo;
        float m_roughness;
};


//
class Dielectric : public Material 
{
public:
        Dielectric(double refraction_index) : m_refraction_index(refraction_index) {}
        Dielectric()  = default;
        ~Dielectric() = default;

        bool scatter(const Ray& r_in, Intersection& intersection, Color& attenuation, Ray& scattered) const override 
        {
                attenuation = Color(1.0, 1.0, 1.0);

                double ri = intersection.front_face ? (1.0 / m_refraction_index) : m_refraction_index;

                Vec3 unit_direction = VecUtils::normal(r_in.direction());
                Vec3 refracted      = VecUtils::refract(unit_direction, intersection.normal, ri);

                // In case refraction angle is >90 the ray should be reflected
                //
                double cos_theta = std::fmin(VecUtils::dot(-unit_direction, intersection.normal), 1.0);
                double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

                bool cannot_refract = ri * sin_theta > 1.0;
                Vec3 direction;

                if (cannot_refract || reflectance(cos_theta, ri) > random_double())
                        direction = VecUtils::reflect(unit_direction, intersection.normal);
                else
                        direction = VecUtils::refract(unit_direction, intersection.normal, ri);

                scattered = Ray(intersection.p, direction);
                //

                return true;
        }

private:
        double m_refraction_index;

        static double reflectance(double cosine, double refraction_index) 
        {
                // Use Schlick's approximation for reflectance.
                auto r0 = (1 - refraction_index) / (1 + refraction_index);

                r0 = r0 * r0;

                return r0 + (1 - r0) * std::pow((1 - cosine), 5);
        }
};