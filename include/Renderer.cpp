#include <iostream>
#include <chrono>
#include <future>
#include <vector>

#include "Renderer.hpp"
#include "Vec3.hpp"
#include "Sphere.hpp"
#include "Ray.hpp"
#include "Object.hpp"
#include "MeshList.hpp"
#include "Interval.hpp"
#include "Utils.hpp"
#include "Material.hpp"


#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "SDL3/SDL.h"
//#include "Windows.h"

#include "ThreadManager.hpp"



Ray Renderer::get_ray(int i, int j) const 
{
        // Get a random ray originating from camera center to a random position
        // inside the (i, j) pixel
        auto offset = sample_square();
        auto pixel_sample = m_camera.viewport().corner_pixel()
                          + m_camera.viewport().pixel_delta_u() * (i + offset.x()) 
                          + m_camera.viewport().pixel_delta_v() * (j + offset.y());

        auto ray_origin    = m_camera.viewport().camera_center();
        auto ray_direction = pixel_sample - ray_origin;

        return Ray(ray_origin, ray_direction);
}

Vec3 Renderer::sample_square() const 
{
        // Returns the vector to a random point in the unit square.
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
        } 
        else 
        {
                return (h - std::sqrt(discriminant) ) / a;
        }
                
        //return (discriminant >= 0);
}

Renderer::Renderer(const Camera& camera): m_camera(camera){}

//Renderer::Renderer(const Viewport& viewport): m_viewport(viewport){}


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

Color Renderer::ray_color(const Ray& r, int bounces, const MeshList& mesh_list)
{
        // *** For debugging only
        // return Color( {0, 0, 0, 0} );

        if (bounces <= 0)
                return Color(0.,0.,0.);

        Intersection intersection;

        if (mesh_list.hit(r, Interval(0.001, INFINITY), intersection)) 
        {
                //Vec3 rgb  = (intersection.normal + Vec3(1,1,1)) * 0.5;

                /* rgb.print(); */

                //Color color(rgb);

                /* color.print_normal_rgb(); */

                // non-lambertian
                //Vec3 direction = VecUtils::random_on_hemisphere(intersection.normal);
                // Lambertian
                /* Vec3 direction = intersection.normal + VecUtils::random_on_hemisphere(intersection.normal);
                Vec3 rgb       = ray_color(Ray(intersection.p, direction), bounces-1, meshlist).normal_rgb() * 0.8; */
                //return Color(rgb);

                Ray scattered;
                Color attenuation;

                if (intersection.mat->scatter(r, intersection, attenuation, scattered))
                {
                        Vec3 rgb     = ray_color(scattered, bounces-1, mesh_list).normal_rgb();
                        Vec3 att_rgb = attenuation.normal_rgb();

                        rgb *= att_rgb;

                        return Color(rgb);
                }

                return Color(0,0,0);

        }

        //return Color(0.85, 0.0, 0.0, 1.0);

        Vec3 unit_direction = VecUtils::normal(r.direction());

        auto a = 0.5*(unit_direction.y() + 1.0);

        Vec3 rgb = Vec3(0.8, 0.8, 1.0) * (1.0 - a) + Vec3(0.2, 0.4, 0.9) * a;

        Color color(rgb);

        return color;
}


void Renderer::set_screen_dims(int screen_x, int screen_y)
{
        m_screen_dims = std::pair<int, int>(screen_x, screen_y);
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

        m_pixels         = std::vector<RGBA>(screen_x*screen_y);
        m_window         = SDL_CreateWindow("raytracer", m_screen_dims.first, m_screen_dims.second, SDL_WINDOW_OPENGL);
        m_renderer       = SDL_CreateRenderer(m_window, NULL);
        m_screen_texture = SDL_CreateTexture(m_renderer, SDL_PixelFormat::SDL_PIXELFORMAT_ABGR8888,
                                             SDL_TextureAccess::SDL_TEXTUREACCESS_STATIC,
                                             screen_x,
                                             screen_y);

        SDL_SetTextureBlendMode(m_screen_texture, SDL_BLENDMODE_BLEND);


        return 0;
}


void Renderer::render_pixel(int x, int y, const MeshList& scene)
{
        auto pixel_center  = m_camera.viewport().corner_pixel()
                           + m_camera.viewport().pixel_delta_u() * x
                           + m_camera.viewport().pixel_delta_v() * y;

        auto ray_direction = pixel_center - m_camera.viewport().camera_center();

        Ray r(m_camera.viewport().camera_center(), ray_direction);

        Color pixel_color(0., 0., 0.);

        Vec3 final_color(0.,0.,0.);

        for (int sample = 0; sample < m_samples_per_pixel; sample++) 
        {
                // random ray of target inside (x, y) pixel
                Ray   r     = get_ray(x, y);
                Color color = ray_color(r, m_max_bounces, scene);

                final_color += color.normal_rgb();
        }
        
        final_color *= m_inverse_samples_per_pixel;

        pixel_color = Color(final_color);

        m_pixels[y * m_screen_dims.first + x] = pixel_color.rgba();
}

// *** TODO
void Renderer::render_row(int row_index, const MeshList& scene)
{
        for(int x = 0; x < m_screen_dims.first; x++)
                render_pixel(x, row_index, scene);
}



// Find a better way instead of having a threaded and a non-threaded function.
void Renderer::render()
{

        std::cout << "Rendering:" << std::endl;

        auto async_render_logger = spdlog::stdout_color_mt<spdlog::async_factory>("async_logger");

        // Set as default logger (optional)
        // spdlog::set_default_logger(async_render_logger);

        auto f = std::make_unique<spdlog::pattern_formatter>("%l %v", spdlog::pattern_time_type::local, std::string(""));  // disable eol

        //async_render_logger->set_level(spdlog::level::info);
        async_render_logger->set_pattern("%v");
        async_render_logger->set_formatter(std::move(f));

        bool running = true;

        SDL_Event e;

        MeshList scene;

        auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.8));
        auto material_center = std::make_shared<Lambertian>(Color(0.9, 0.2, 0.0));
        auto material_left   = std::make_shared<Dielectric>(1.50);
        auto material_right  = std::make_shared<Metal>(Color(0.2, 0.8, 0.2));

        scene.add(std::make_shared<Sphere>(Vec3( 0.0, -100.5, -1.0), 100.0, material_ground));
        scene.add(std::make_shared<Sphere>(Vec3( 0.0,    0.0, -1.2),   0.5, material_center));
        scene.add(std::make_shared<Sphere>(Vec3(-1.0,    0.0, -1.0),   0.5, material_left));
        scene.add(std::make_shared<Sphere>(Vec3( 1.0,    0.0, -1.0),   0.5, material_right));


        // Start timer
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::future<void>> futures;

        // *** Replace with an update function 
        // Updating pixels
        for (int y = 0; y < m_screen_dims.second; ++y) 
        {
                const size_t p = (int)(((double)y/m_screen_dims.second)*100);

                std::string progress(p, '|');

                std::string percentage = fmt::format("{:.2f}", ((double)y/m_screen_dims.second)*100);

                async_render_logger->info("\r[{}]{}", percentage, progress);

                render_row(y, scene);
        } 

        std::cout << "Done." << std::endl;

        auto        end             = std::chrono::high_resolution_clock::now();
        auto        elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        std::string elapsed_str     = fmt::format("{}", elapsed_seconds);

        async_render_logger->info("Elapsed: {}s", elapsed_str); 


        while (running) 
        {
                while (SDL_PollEvent(&e)) 
                {
                        if (e.type == SDL_EVENT_QUIT) 
                        {
                                running = false;
                        }
                }

                SDL_UpdateTexture(m_screen_texture, nullptr, m_pixels.data(), m_screen_dims.first * sizeof(RGBA));
                SDL_RenderClear  (m_renderer);
                SDL_RenderTexture(m_renderer, m_screen_texture, nullptr, nullptr);
                SDL_RenderPresent(m_renderer);
        }

        SDL_DestroyTexture (m_screen_texture);
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow  (m_window);
        SDL_Quit();

}
void Renderer::threaded_render()
{

        std::cout << "Rendering:" << std::endl;

        auto async_render_logger = spdlog::stdout_color_mt<spdlog::async_factory>("async_logger");

        auto f = std::make_unique<spdlog::pattern_formatter>("%l %v", spdlog::pattern_time_type::local, std::string(""));  // disable eol

        //async_render_logger->set_level(spdlog::level::info);
        async_render_logger->set_pattern("%v");
        async_render_logger->set_formatter(std::move(f));


        SDL_Event e;

        MeshList scene;

        auto material_ground = std::make_shared<Lambertian>(Color(0.7, 0.7, 0.7));
        auto material_center = std::make_shared<Lambertian>(Color(1.0, 0.1, 0.1));
        auto material_left   = std::make_shared<Dielectric>(1.50);
        auto material_bubble = std::make_shared<Dielectric>(1.00 / 1.50);
        auto material_right  = std::make_shared<Metal>(Color(0.1, 0.8, 0.2), 0.1);

        scene.add(std::make_shared<Sphere>(Vec3( 0.0, -100.5, -1.0), 100.0, material_ground));
        scene.add(std::make_shared<Sphere>(Vec3( 0.0,    0.0, -1.2),   0.5, material_center));
        scene.add(std::make_shared<Sphere>(Vec3(-1.0,    0.0, -1.0),   0.5, material_left));
        scene.add(std::make_shared<Sphere>(Vec3(-1.0,    0.0, -1.0),   0.4, material_bubble));
        scene.add(std::make_shared<Sphere>(Vec3( 1.0,    0.0, -1.0),   0.5, material_right));

        // Start timer
        auto start = std::chrono::high_resolution_clock::now();

        ThreadManager thread_manager;
        std::vector<std::future<void>> futures;

        async_render_logger->info("-- MAX CONCURRENT THREADS: {}", thread_manager.max_threads());

        bool done            = false;
        bool time_msg        = false;
        bool thread_launched = false;

        size_t y_count = 0;

        bool running = true;

        while (running) 
        {

                while (SDL_PollEvent(&e)) 
                {
                        if (e.type == SDL_EVENT_QUIT) 
                        {
                                running = false;
                        }
                }

                const size_t p = (int)(((double)y_count/m_screen_dims.second)*100);

                std::string progress(p, '|');

                std::string percentage = fmt::format("{:.2f}", ((double)y_count/m_screen_dims.second)*100);

                if(!done)
                {
                        async_render_logger->info("\r[{}]{}", percentage, progress);
                }
                
                // launch thread
                // thread_manager limits the concurrent threads
                // * FIND A BETTER WAY
                // ==========================================
                // ==========================================
                thread_manager.acquire();

                futures.emplace_back(std::async(std::launch::async, [=, &scene, &thread_manager]() 
                {
                        render_row(y_count, scene);
                        thread_manager.release();
                }
                ));

                if(y_count < m_screen_dims.second - 1) 
                        y_count++;
                else 
                        done = true;
                
                if (done && !thread_launched)
                {
                        thread_launched = !thread_launched;

                        for (auto& f : futures) f.get();
                }

                if(done && !time_msg)
                {
                        time_msg = !time_msg;

                        std::cout << "Done." << std::endl;
                        std::cout << std::endl << std::flush;

                        auto        end             = std::chrono::high_resolution_clock::now();
                        auto        elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
                        std::string elapsed_str     = fmt::format("{}", elapsed_seconds);

                        async_render_logger->info("Elapsed: {}s", elapsed_str);

                }
                // ==========================================
                // ==========================================


                SDL_UpdateTexture(m_screen_texture, nullptr, m_pixels.data(), m_screen_dims.first * sizeof(RGBA));
                SDL_RenderClear  (m_renderer);
                SDL_RenderTexture(m_renderer, m_screen_texture, nullptr, nullptr);
                SDL_RenderPresent(m_renderer);
        }

        SDL_DestroyTexture(m_screen_texture);
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();

}