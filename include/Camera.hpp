#pragma once

#include "Object.hpp"


class Camera: public Object
{
        public:
        void set_aspect_ratio(float);

         Camera(Vec3 pos);
         Camera(Vec3 pos, Vec3 size);
         Camera() = default;
        ~Camera() = default;

        private:
        float aspect_ratio;
        float m_focal_length;
        Vec3 m_target_position;

};