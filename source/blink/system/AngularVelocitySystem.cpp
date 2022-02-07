/**

    @file      AngularVelocitySystem.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      8.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "AngularVelocitySystem.h"
#include "../component/Components.h"

namespace blink
{
    bool AngularVelocitySystem::initialize(flecs::world& world)
    {
        world.system<Rotation&, const AngularVelocity&>().each(
            [](flecs::entity e, Rotation& rot, const AngularVelocity& vel)
            {
                //
                rot.value *= glm::quat(vel.value * e.delta_time());
            });

        return true;
    }

    void AngularVelocitySystem::terminate(flecs::world& world)
    {
        // 
    }

} // namespace blink
