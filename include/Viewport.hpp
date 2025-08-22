#pragma once

#include "Vec3.hpp"

class Viewport
{
      public:
	const Vec3& camera_center() const { return m_camera_center; }
	const Vec3& corner_pixel()  const { return m_corner_pixel; }
	const Vec3& upper_pixel()   const { return m_upper_left; }
	const Vec3& pixel_delta_u() const { return m_pixel_delta_u; }
	const Vec3& pixel_delta_v() const { return m_pixel_delta_v; }

	Viewport(Vec3     camera_center,
	         Vec3     camera_u,
	         Vec3     camera_v,
	         Vec3     camera_w,
	         double   height,
	         double   aspect_ratio,
	         unsigned screen_width,
	         unsigned screen_height,
	         double   focal_length,
	         double   vfov);

	Viewport(double);
	Viewport()  = default;
	~Viewport() = default;

      private:
	double m_height, m_width;
	Vec3   m_camera_center;
	Vec3   m_pixel_delta_u, m_pixel_delta_v;
	Vec3   m_viewport_u, m_viewport_v;
	Vec3   m_corner_pixel;
	Vec3   m_upper_left;
};