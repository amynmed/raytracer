#pragma once

#include "Color.hpp"


enum class MATERIAL_TYPE
{
        PHONG,
        BLINN,
        LAMBERT
};

class Material
{
        public:
        void set_color(Color);

        Material() = default;
        virtual ~Material() = default; 


        private:
        Color color;
};