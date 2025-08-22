

#include <iostream>

#include "Camera.hpp"
#include "Renderer.hpp"
#include "Vec3.hpp"

// *TODO
// ==================
// - FIX COLOR CLASS (add operations support)
//
// ==================

int main()
{
	// FOR DEBUGGING
	constexpr double aspect_ratio  = 16.0 / 9.0;
	constexpr int    screen_height = 400;
	constexpr int    screen_width  = screen_height * aspect_ratio;

	// double focal_length = 1;
	Vec3 initial_camera_position(0, 0, 0);

	Camera camera(initial_camera_position, screen_width, aspect_ratio);
	camera.set_aspect_ratio(aspect_ratio);
	camera.look_at(Vec3(0, 0, -1));
	camera.look_from(Vec3(-2, 2, 1));

	Renderer renderer(camera);

	renderer.init(screen_width, screen_height);
	renderer.threaded_render();

	return 0;
}