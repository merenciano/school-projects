#include "cube-controller.h"

#include "core/common-defs.h"
#include "core/input.h"
#include "core/manager.h"
#include "core/window.h"
#include "ecs/components/velocity.h"
#include "ecs/entity.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace leep
{
    CubeController::CubeController(String name)
    {
        name_ = name;
        jump_power_ = 1.0f;
        down_power_ = 1.0f;
    }

    CubeController::CubeController(String name, float jump_power, float down_power)
    {
        name_ = name;
        jump_power_ = jump_power; 
        down_power_ = down_power;
    }

    void CubeController::executeSystem() const
    {
        float delta = GM.delta_time();
        Velocity &v = Entity::GetEntity(name_).getComponent<Velocity>();

        if (GM.input().isButtonDown(Button::UP))
        {
            v.velocity_.y = jump_power_;
        }

        if (GM.input().isButtonPressed(Button::DOWN))
        {
            v.velocity_.y -= down_power_ * delta;
        }
    }
}