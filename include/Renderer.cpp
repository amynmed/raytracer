#include <iostream>

#include "Renderer.hpp"
#include "Vec3.hpp"

#include "SDL3/SDL.h"
#include "Sphere.hpp"
#include "Ray.hpp"
#include "Object.hpp"
#include "MeshList.hpp"
#include "Interval.hpp"
#include "Utils.hpp"

#include "Windows.h"

Ray Renderer::get_ray(int i, int j) const 
{
        // Get a random ray originating from camera center to a random position
        // inside the (i, j) pixel

        auto offset = sample_square();
        auto pixel_sample = m_viewport.corner_pixel()
                          + m_viewport.pixel_delta_u() * (i + offset.x()) 
                          + m_viewport.pixel_delta_v() * (j + offset.y());

        auto ray_origin    = m_viewport.camera_center();
        auto ray_direction = pixel_sample - ray_origin;

        return Ray(ray_origin, ray_direction);
}

Vec3 Renderer::sample_square() const 
{
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

// FOR DEBUG
double hit_sphere(const Vec3& center, double radius, const Ray& r) 
{
        Vec3 oc = center - r.origin();

        auto a = VecUtils::dot(r.direction(), r.direction());
        auto h = VecUtils::dot(r.direction(), oc);
        auto c = VecUtils::dot(oc, oc) - radius*radius;


        auto discriminant = h*h - a*c;


        if (discriminant < 0) 
        {
                return -1.0;
        } else 
        {
                return (h - std::sqrt(discriminant) ) / a;
        }
                
        //return (discriminant >= 0);
}

Renderer::Renderer(const Viewport& viewport): m_viewport(viewport){}


/* Color Renderer::ray_color(const Ray& r)
{
        // *** For debugging only
        // return Color( {0, 0, 0, 0} );

        Intersection intersection;
        Sphere sphere(Vec3(0, 0, -1), 0.3);

        //auto t = hit_sphere(Vec3(0, 0, -1), 0.3, r);
        auto t = sphere.hit(r, Interval(0, INFINITY), intersection);

        if (t > 0.0) 
        {
                Vec3 N = VecUtils::normal(r.position(t) - Vec3(0, 0, -1));

                return Color((N.x()+1)*0.5, (N.y()+1)*0.5, (N.z()+1)*0.5);
        }


        //return Color(0.85, 0.0, 0.0, 1.0);

        Vec3 unit_direction = VecUtils::normal(r.direction());

        auto a = 0.5*(unit_direction.y() + 1.0);

        Vec3 rgb = Vec3(0.3, 0.3, 1.0) * (1.0 - a) + Vec3(0.0, 0.2, 0.8) * a;

        RGB rgb = {rgb.x() * 255, rgb.y() * 255, rgb.z() * 255};

        Color color(rgb);

        return color;
} */

Color Renderer::ray_color(const Ray& r, const MeshList& meshlist)
{
        // *** For debugging only
        // return Color( {0, 0, 0, 0} );

        Intersection intersection;

        if (meshlist.hit(r, Interval(0, INFINITY), intersection)) 
        {
                Vec3 rgb  = (intersection.normal + Vec3(1,1,1)) * 0.5;

                /* rgb.print(); */

                Color color(rgb);

                /* color.print_normal_rgb(); */


                return color;
        }


        //return Color(0.85, 0.0, 0.0, 1.0);

        Vec3 unit_direction = VecUtils::normal(r.direction());

        auto a = 0.5*(unit_direction.y() + 1.0);

        Vec3 rgb = Vec3(0.3, 0.3, 1.0) * (1.0 - a) + Vec3(0.0, 0.2, 0.8) * a;

        Color color(rgb);

        return color;
}


void Renderer::set_screen_dims(int screen_x, int screen_y)
{
        screen_dims = std::pair<int, int>(screen_x, screen_y);
}

int Renderer::init(int screen_x, int screen_y)
{
        set_screen_dims(screen_x, screen_y);

        if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

        m_inverse_samples_per_pixel = 1.0 / m_samples_per_pixel;

        printf("INVERSE SAMPLES: %.4f\n", m_inverse_samples_per_pixel);

        pixels         = std::vector<RGBA>(screen_x*screen_y);
        window         = SDL_CreateWindow("raytracer", screen_dims.first, screen_dims.second, SDL_WINDOW_OPENGL);
        renderer       = SDL_CreateRenderer(window, NULL);
        screen_texture = SDL_CreateTexture(renderer, SDL_PixelFormat::SDL_PIXELFORMAT_ABGR8888, SDL_TextureAccess::SDL_TEXTUREACCESS_STATIC, screen_x, screen_y);

        SDL_SetTextureBlendMode(screen_texture, SDL_BLENDMODE_BLEND);

        

        // Testing a random texture
        /* for (int y = 0; y < screen_y; ++y) 
        {
                for (int x = 0; x < screen_x; ++x) 
                {
                        RGBA color;

                        float px = (float)x / screen_x;
                        float py = (float)y / screen_y;

                          //std::cout << px << py << std::endl;

                        float xclr = px * 255;
                        float yclr = py * 255;

                        color.R = static_cast<uint8_t>(xclr);
                        color.G = static_cast<uint8_t>(yclr); 
                        color.R = 0;
                        color.G = 0;
                        color.B = 200 - static_cast<uint8_t>(yclr * 0.5);
                        color.A = 255;

                        pixels[y * screen_x + x] = color;
                }
        } */
        /* for (int y = 0; y < screen_y; ++y) 
        {
                for (int x = 0; x < screen_x; ++x) 
                {
                        RGBA color;

                        color.R = x % 256;
                        color.G = y % 256;
                        color.B = (x + y * 2) % 256;
                        color.A = 255;

                        pixels[y * screen_x + x] = color;
                }
        } */

        // SDL_UpdateTexture(screen_texture, nullptr, pixels.data(), screen_x * sizeof(RGBA));

        return 0;
}






void Renderer::render()
{

        bool running = true;

        SDL_Event e;


        std::cout << "Rendering ..." << std::endl;


        MeshList scene;

        scene.add(std::make_shared<Sphere>(Vec3(0, 0, -1), 0.5));
        scene.add(std::make_shared<Sphere>(Vec3(0, -100.5, -1), 100));

        // *** Replace with an update function 
        // Updating pixels
        for (int y = 0; y < screen_dims.second; ++y) 
        {
                const size_t p = (int)(((double)y/screen_dims.second)*100);

                std::string progress(p, '-');

                std::cout << "\rScanlines remaining: " << ((double)y/screen_dims.second)*100 << progress << std::flush;

                for (int x = 0; x < screen_dims.first; ++x) 
                {
                        auto pixel_center  = m_viewport.corner_pixel() + (m_viewport.pixel_delta_u() * x) + (m_viewport.pixel_delta_v() * y);
                        auto ray_direction = pixel_center - m_viewport.camera_center();

                        Ray r(m_viewport.camera_center(), ray_direction);

                        //Color pixel_color = ray_color(r, scene);
                        Color pixel_color(0., 0., 0.);

                        Vec3 final_color(0.,0.,0.);

                        for (int sample = 0; sample < m_samples_per_pixel; sample++) 
                        {
                                // random ray of target inside (x, y) pixel
                                Ray   r     = get_ray(x, y);
                                Color color = ray_color(r, scene);

                                /* color.print_normal_rgb();
                                color.print_rgba(); */

                                final_color += color.normal_rgb();

                        }

                        //std::cout << "FINAL PIXEL: " << final_color.x() << ", " << final_color.y() << ", " << final_color.z() << std::endl;

                        
                        final_color *= m_inverse_samples_per_pixel;

                        pixel_color = Color(final_color);

                        /* pixel_color.print_rgba(); */

                        pixels[y * screen_dims.first + x] = pixel_color.rgba();
                }

                
        }

        std::cout << "Done." << std::endl;


        while (running) 
        {

                while (SDL_PollEvent(&e)) 
                {
                        if (e.type == SDL_EVENT_QUIT) 
                        {
                                running = false;
                        }
                }

                

                SDL_UpdateTexture(screen_texture, nullptr, pixels.data(), screen_dims.first * sizeof(RGBA));

                SDL_RenderClear(renderer);
                SDL_RenderTexture(renderer, screen_texture, nullptr, nullptr);
                SDL_RenderPresent(renderer);
        }

        SDL_DestroyTexture(screen_texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

}