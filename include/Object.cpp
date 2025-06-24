
#include "Object.hpp"


Object::Object(Vec3 p_position): m_position(p_position){}
Object::Object(Vec3 p_position, Vec3 p_size): m_position(p_position), m_size(p_size) {}