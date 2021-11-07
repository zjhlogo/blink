/**

    @file      RotationSystem.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#include "RotationSystem.h"

#include <blink/component/Components.h>

bool RotationSystem::initialize(flecs::world& world)
{
    world.system<blink::Rotation, const blink::StaticModel>().each(
        [](flecs::entity e, blink::Rotation& rot, const blink::StaticModel& sm)
        {
            float dt = e.delta_time();
            rot.value = glm::rotate(rot.value, 0.2f * dt, glm::vec3(0.0f, 1.0f, 0.0f));
        });

    return true;
}
