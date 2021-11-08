/**

    @file      LinearVelocitySystem.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      8.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "LinearVelocitySystem.h"
#include "../component/Components.h"

namespace blink
{
    bool LinearVelocitySystem::initialize(flecs::world& world)
    {
        world.system<Position&, const LinearVelocity&>().each([](flecs::entity e, Position& pos, const LinearVelocity& vel)
                                                              { pos.value += vel.value * e.delta_time(); });
        return true;
    }
} // namespace blink
