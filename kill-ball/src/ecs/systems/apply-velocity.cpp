#include "apply-velocity.h"

#include "core/common-defs.h"
#include "core/manager.h"
#include "ecs/components/ltransform.h"
#include "ecs/components/velocity.h"
#include "ecs/entity.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace leep
{
    void ApplyVelocity::executeSystem() const
    {
        float delta = GM.delta_time();
#ifdef LEEP_DEBUG
        uint64_t mask = ((1 << COMP_VELOCITY) | (1 << COMP_LTRANSFORM));
        LEEP_ASSERT((container_.mask() & mask) == mask, "This container is not valid for this system.");
#endif
        for (auto &chunk : container_.blocks_)
        {
            LTransform *tr_array = chunk->template component<LTransform>();
            Velocity *vl_array = chunk->template component<Velocity>();
            for(int32_t i = 0; i < chunk->last_; ++i)
            {
                glm::vec3 &vel = vl_array[i].velocity_;
                glm::mat4 &tr = tr_array[i].transform_;
                tr = glm::translate(tr, vel * delta);
                // Not falling below Y=-1.0
                if (tr[3][1] < -1.0f)
                {
                    tr[3][1] = -1.0f;
                    vel.y = 0.0f;
                }
                else if (tr[3][1] > 2.3f)
                {
                    tr[3][1] = 2.3f;
                    vel.y = 0.0f;
                }
            }
        }
    }
}