#pragma once

#include "Vec3.hpp"

class Ray
{
        public:
        const Vec3& origin() const;
        const Vec3& direction() const;
        
         Ray(Vec3, Vec3);
         Ray() = default;
        ~Ray() = default;

        Vec3 position(double t) const;

        private:
        Vec3 m_origin, m_direction;
};