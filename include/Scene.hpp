#pragma once

#include <vector>
#include "Object.hpp"
#include "Camera.hpp"


class Scene
{

public:
        void add_object(Object &object);

         Scene() = default;
        ~Scene() = default;


private:
        std::vector<Object> objects;
        Camera active_camera;
        
};