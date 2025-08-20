#pragma once

#include "Object.hpp"

// **TODO: extend Targetable class
//
class Camera: public Object
{
public:
        void set_aspect_ratio(float);

         Camera(Vec3 pos);
         Camera(Vec3 pos, Vec3 size);
         Camera() = default;
        ~Camera() = default;

private:
        Viewport& m_viewport;
        float aspect_ratio;
        float m_focal_length;
        Vec3 m_target_position;

        double vfov     = 90;            
        Vec3   lookfrom = Vec3(0,0,0);   
        Vec3   lookat   = Vec3(0,0,-1);  
        Vec3   vup      = Vec3(0,1,0);   

};