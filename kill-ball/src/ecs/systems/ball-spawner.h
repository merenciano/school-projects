#ifndef __LEEP_ECS_SYSTEMS_BALL_SPAWNER_H__
#define __LEEP_ECS_SYSTEMS_BALL_SPAWNER_H__

#include "core/string.h"
#include "ecs/system.h"

namespace leep
{
    class Material;

    class BallSpawner : public System
    {
    public:
        static const int32_t kBallCount = 30;
        BallSpawner() : interval_(1.0f) {}
        BallSpawner(float interval) : interval_(interval) {}
        ~BallSpawner() = default;
        virtual void executeSystem() const override;
        void spawnBall(const Material &mat, float speed) const;
    private:
        float interval_;
    };
}

#endif 
