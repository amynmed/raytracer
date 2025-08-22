

#include "Viewport.hpp"
#include "Utils.hpp"

Viewport::Viewport(Vec3     camera_center,
                   Vec3     camera_u,
                   Vec3     camera_v,
                   Vec3     camera_w,
                   double   height,
                   double   aspect_ratio,
                   unsigned screen_width,
                   unsigned screen_height,
                   double   focal_length,
                   double   vfov)
{
	m_camera_center = camera_center;

	auto theta      = deg_to_rad(vfov);
	auto h          = std::tan(theta / 2);

	m_height        = 2 * h * focal_length;
	m_width         = m_height * (double(screen_width) / screen_height);

	m_viewport_u    = camera_u * m_width;
	m_viewport_v    = -camera_v * m_height;

	m_pixel_delta_u = m_viewport_u / screen_width;
	m_pixel_delta_v = m_viewport_v / screen_height;

	m_upper_left    = camera_center - (camera_w * focal_length) - m_viewport_u / 2 -
	               m_viewport_v / 2;

	m_corner_pixel = m_upper_left + (m_pixel_delta_u + m_pixel_delta_v) * 0.5;
}