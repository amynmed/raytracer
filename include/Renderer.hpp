
#include <vector>

#include "Color.hpp"
#include "Camera.hpp"
#include "World.hpp"

class Renderer
{
        void set_world(World);
        void set_camera(Camera);
        void render();

        private:
        std::pair<int, int> screen_dims;
        std::vector<Color> img;

};