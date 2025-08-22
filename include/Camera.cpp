#include "Camera.hpp"
#include "Vec3.hpp"



void Camera::set_aspect_ratio(float aspect_ratio)
{
	m_aspect_ratio = aspect_ratio;
}

/* Camera::Camera(Vec3 position)
{
        Object::Object(position);

        m_w = VecUtils::normal(m_lookfrom - m_lookat);
        m_u = VecUtils::normal(VecUtils::cross(m_vup, m_w));
        m_v = VecUtils::cross(m_w, m_u);

        init();

} */

Camera::Camera(Vec3 position, int screen_width, float aspect_ratio)
    : Object(position), m_screen_width(screen_width), m_aspect_ratio(aspect_ratio)
{

	m_w = VecUtils::normal(m_lookfrom - m_lookat);
	m_u = VecUtils::normal(VecUtils::cross(m_vup, m_w));
	m_v = VecUtils::cross(m_w, m_u);

	init();
}

void Camera::init()
{
	auto focal_length = (m_lookfrom - m_lookat).magnitude();

        // ** TODO: refactor
        // take camera ref instead
	m_viewport = Viewport(m_lookfrom,
	                      m_u,
	                      m_v,
	                      m_w,
	                      2.0,
	                      m_aspect_ratio,
	                      m_screen_width,
	                      m_screen_width / m_aspect_ratio,
	                      focal_length,
	                      m_vfov);
}
