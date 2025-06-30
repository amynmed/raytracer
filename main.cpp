

#include <iostream>

#include "Renderer.hpp"
#include "Camera.hpp"
#include "Vec3.hpp"



// *TODO
// ==================
// - FIX COLOR CLASS (add operations support)
//
// ==================

int main()
{
        // FOR DEBUGGING
        constexpr double aspect_ratio = 4.0/3.0;
        constexpr int screen_height   = 500;
        constexpr int screen_width    = screen_height * aspect_ratio;



        double focal_length = 1;
        Vec3 initial_camera_position(0, 0, 0);

        Camera camera(initial_camera_position);
        camera.set_aspect_ratio(aspect_ratio);


        Viewport viewport(initial_camera_position, 2.0, aspect_ratio, screen_width, screen_height, focal_length);

        Renderer renderer(viewport);

        renderer.init(screen_width, screen_height);
        renderer.threaded_render();
        

        


        return 0;
}