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