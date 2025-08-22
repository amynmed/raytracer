
#include "Color.hpp"
#include "Vec3.hpp"
#include <cassert>

// Color::Color(RGB rgb) : m_rgb(rgb) {}

Color::Color(double r, double g, double b)
{
	assert(r <= 1.0 && r >= 0.0);
	assert(g <= 1.0 && g >= 0.0);
	assert(b <= 1.0 && b >= 0.0);

	m_normal_rgb = Vec3(r, g, b);
	// m_normal_rgb = Vec3(linear_to_gamma(r), linear_to_gamma(g), linear_to_gamma(b));

	m_rgba.R = static_cast<uint8_t>(r * 255.);
	m_rgba.G = static_cast<uint8_t>(g * 255.);
	m_rgba.B = static_cast<uint8_t>(b * 255.);
	m_rgba.A = 255;
}

Color::Color(Vec3 rgb) : Color(rgb.x(), rgb.y(), rgb.z()) {}