#ifndef __LEEP_ECS_SYSTEMS_CUBE_CONTROLLER_H__
#define __LEEP_ECS_SYSTEMS_CUBE_CONTROLLER_H__

#include "core/string.h"
#include "ecs/system.h"

namespace leep
{
    class CubeController : public System
    {
    public:
        CubeController(String name);
        CubeController(String name, float jump_power, float down_power);
        ~CubeController() = default;
        virtual void executeSystem() const override;
    private:
        String name_;
        float jump_power_;
        float down_power_;
    };
}

#endif 
