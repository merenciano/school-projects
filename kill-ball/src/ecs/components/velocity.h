// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_ECS_COMPONENTS_VELOCITY_H__
#define __LEEP_ECS_COMPONENTS_VELOCITY_H__

#include "ecs/component.h"
#include <glm/glm.hpp>

namespace leep 
{
    struct Velocity : public Component
    {
        static const CompType s_type = COMP_VELOCITY;
        Velocity() : Component(COMP_VELOCITY) { velocity_ = glm::vec3(0.0f); }
        Velocity& operator=(const Velocity &other) { velocity_ = other.velocity_; return *this; }
        glm::vec3 velocity_;
    };
}

#endif 
