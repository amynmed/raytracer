
#include "Vec3.hpp"

class Object
{

        Vec3 get_position();
        void set_position(Vec3);
        void scale(float);

        ~Object() = default;

        private:
        float m_x, m_y, m_z;
        float m_sx, m_sy, m_sz;
};