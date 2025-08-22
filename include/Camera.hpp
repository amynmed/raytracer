#pragma once

#include "Object.hpp"
#include "Viewport.hpp"

// **TODO: extend Targetable class
// ===============================
class Camera : public Object
{
public:
	void init();

	void set_aspect_ratio(float);

	void set_vup  (Vec3 vup)       { m_vup      = vup; }
	void look_at  (Vec3 look_at)   { m_lookat   = look_at; }
	void look_from(Vec3 look_from) { m_lookfrom = look_from; }

	// Camera(Vec3 pos);
	// Camera(Vec3 pos, Vec3 size);
	Camera(Vec3 pos, int screen_width, float aspect_ratio);
	Camera()  = default;
	~Camera() = default;

	const Viewport& viewport() const { return m_viewport; }

private:
	Viewport m_viewport;
	int      m_screen_width;
	float    m_aspect_ratio;
	float    m_focal_length;
	Vec3     m_target_position;
	Vec3     m_u, m_v, m_w;

	double m_vfov     = 30;
	Vec3   m_lookfrom = Vec3(-3, 1, 1);
	Vec3   m_lookat   = Vec3(0, 0, -1);
	Vec3   m_vup      = Vec3(0, 2, 0);
};