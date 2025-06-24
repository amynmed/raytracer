


#include "Viewport.hpp"



Viewport::Viewport(Vec3 camera_center, double height, double aspect_ratio, unsigned screen_width, unsigned screen_height, double focal_length)
{

        m_camera_center = camera_center; 

        m_height = height;
        m_width  = m_height * aspect_ratio;
        
        
        m_viewport_u = Vec3(m_width, 0, 0);
        m_viewport_v = Vec3(0, -m_height, 0);


        m_pixel_delta_u = m_viewport_u / screen_width;
        m_pixel_delta_v = m_viewport_v / screen_height;

        m_upper_left   = camera_center - Vec3(0, 0, focal_length) - m_viewport_u/2 - m_viewport_v/2;
        m_corner_pixel = m_upper_left + (m_pixel_delta_u + m_pixel_delta_v) * 0.5;
}