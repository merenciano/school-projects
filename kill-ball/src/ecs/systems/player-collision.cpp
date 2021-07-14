#include "player-collision.h"

#include "core/common-defs.h"
#include "core/manager.h"
#include "ecs/components/gtransform.h"
#include "ecs/components/ltransform.h"
#include "ecs/components/velocity.h"
#include "ecs/systems/ball-spawner.h" //  kBallCount
#include "ecs/entity.h"

#include "kill-ball.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace leep
{
    void PlayerCollision::executeSystem() const
    {
        // TODO: This system is super ugly and hardcoded ad-hoc for this
        //       particular case... It should be refactored if I have time.

        Entity first = Entity::GetEntity("Ball_0");
        GTransform *tr_arr = first.componentPtr<GTransform>();
        glm::vec3 ploc = Entity::GetEntity("Player").getComponent<GTransform>().location();
        float yvel = Entity::GetEntity("Player").getComponent<Velocity>().velocity_.y;
        float xmin = ploc.x - 0.5f;
        float xmax = ploc.x + 0.5f;
        float ymin = ploc.y - 0.5f;
        float ymax = ploc.y + 0.5f;
        float zmin = ploc.z - 0.5f;
        float zmax = ploc.z + 0.5f;

        for (int32_t i = 0; i < BallSpawner::kBallCount; ++i)
        {
            glm::vec3 bloc = (tr_arr + i)->location();
            glm::vec3 closest;
            closest.x = glm::max(xmin, glm::min(bloc.x,xmax));
            closest.y = glm::max(ymin, glm::min(bloc.y,ymax));
            closest.z = glm::max(zmin, glm::min(bloc.z,zmax));

            float dist = glm::sqrt((closest.x - bloc.x) * (closest.x - bloc.x) +
                                   (closest.y - bloc.y) * (closest.y - bloc.y) +
                                   (closest.z - bloc.z) * (closest.z - bloc.z));

            // 0.3 is the sphere radius (hardcoded)
            if (dist < 0.3f)
            {
                killball::ConPkg pkg;
                if (yvel < 0.0f && (ploc.y > bloc.y + 0.4f || ploc.x > bloc.x))
                {
                    LTransform *bltr = first.componentPtr<LTransform>() + i;
                    bltr->transform_ = glm::translate(bltr->transform_, glm::vec3(-50.0f, 0.0f, 0.0f));

                    pkg.type_ = killball::ConDataType::SPAWN_BALL;
                    con_->send(pkg);
                    return;
                }
                else
                {
                    pkg.type_ = killball::ConDataType::DEATH;
                    con_->send(pkg);
                }
            }
        }

    }
}