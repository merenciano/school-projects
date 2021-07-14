#ifndef __LEEP_ECS_SYSTEMS_PLAYER_COLLISION_H__
#define __LEEP_ECS_SYSTEMS_PLAYER_COLLISION_H__

#include "core/string.h"
#include "networking/socket/connection.h"
#include "ecs/system.h"

namespace leep
{
    class PlayerCollision : public System
    {
    public:
        PlayerCollision() = delete;
        PlayerCollision(Connection *con) : con_(con) {}
        ~PlayerCollision() = default;
        virtual void executeSystem() const override;
    private:
        Connection *con_;
    };
}

#endif 
