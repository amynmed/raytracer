
#include "Camera.hpp"
#include "Vec3.hpp"

void Camera::set_aspect_ratio(float p_aspect_ratio)
{
        aspect_ratio = p_aspect_ratio;
}

Camera::Camera(Vec3 p_position)
{
        Object::Object(p_position);
}

Camera::Camera(Vec3 p_position, Vec3 p_size)
{
        Object::Object(p_position, p_size);
}

