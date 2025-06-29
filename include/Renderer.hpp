#pragma once

#include <vector>

#include "Color.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "Viewport.hpp"
#include "Ray.hpp"
#include "MeshList.hpp"

#include "SDL3/SDL.h"



class Renderer
{
        //
        public:
        int init(int, int);
        void set_screen_dims(int, int);
        void set_world(Scene);
        void set_camera(Camera);
        void set_viewport(Viewport);
        void update();
        void render();
        void set_samples_per_pixel(unsigned n_samples) {m_samples_per_pixel = n_samples; }

        Ray get_ray(int i, int j) const;
        Vec3 sample_square() const;

        Color ray_color(const Ray&);
        Color ray_color(const Ray&, int, const MeshList&);

         Renderer(const Viewport&);
         Renderer() = default;
        ~Renderer() = default;

        //
        private:
        std::pair<int, int> screen_dims;
        std::vector<RGBA> pixels;

        const Viewport& m_viewport;

        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *screen_texture;

        // AA
        int m_samples_per_pixel = 50;
        double m_inverse_samples_per_pixel;

        int m_max_bounces = 50;

};

