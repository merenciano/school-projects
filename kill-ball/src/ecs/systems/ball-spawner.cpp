#include "ball-spawner.h"

#include "core/common-defs.h"
#include "core/manager.h"
#include "ecs/components/ltransform.h"
#include "ecs/components/velocity.h"
#include "ecs/components/drawable.h"
#include "ecs/entity.h"

#include "kill-ball.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

// const method....
static int32_t next = 0;
static float timer = 0.0f;

namespace leep
{
    void BallSpawner::spawnBall(const Material &m, float speed) const
    {
        String name = "Ball_" + ToString(next);
        ++next %= kBallCount;
        Drawable &dw = Entity::GetEntity(name).getComponent<Drawable>();
        dw.material_ = m;
        LTransform &tr = Entity::GetEntity(name).getComponent<LTransform>();
        Velocity &v = Entity::GetEntity(name).getComponent<Velocity>();
        v.velocity_.x = -speed;
        float height = glm::linearRand<int32_t>(0, 40) / 10.0f - 2.0f;
        tr.setLocation(glm::vec3(10.0f, height, -5.0f));
    }

    void BallSpawner::executeSystem() const
    {
        timer += GM.delta_time();
        if (timer > interval_) 
        {
            timer = 0.0f;
            spawnBall(killball::GetGame().data_.mat_ball_, 12.0f);
        }
    }
}