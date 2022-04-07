/**

    @file      LinearVelocitySystem.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      8.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "LinearVelocitySystem.h"
#include "../EcsWorld.h"
#include "../components/Components.h"

namespace blink
{
    bool LinearVelocitySystem::initialize()
    {
        m_ecsWorld->getWorld()
            .system<Position, const LinearVelocity>("apply linear velocity")
            .each(
                [](flecs::entity e, Position& pos, const LinearVelocity& vel)
                {
                    //
                    pos.value += vel.value * e.delta_time();
                });

        return true;
    }

    void LinearVelocitySystem::terminate()
    {
        //
    }
} // namespace blink
