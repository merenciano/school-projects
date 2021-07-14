#ifndef __LEEP_ECS_SYSTEMS_APPLY_VELOCITY_H__
#define __LEEP_ECS_SYSTEMS_APPLY_VELOCITY_H__

#include "core/string.h"
#include "ecs/system.h"

namespace leep
{
    class ApplyVelocity : public System
    {
    public:
        ApplyVelocity() = delete;
        ApplyVelocity(class EntityContainer &container) : container_(container) {}
        ~ApplyVelocity() = default;
        class EntityContainer &container_;
        virtual void executeSystem() const override;
    };
}

#endif 
