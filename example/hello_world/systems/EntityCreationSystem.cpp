/**

    @file      EntityCreationSystem.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/

#include "EntityCreationSystem.h"

#include <blink/component/Components.h>

EntityCreationSystem::EntityCreationSystem(const glm::vec2& surfaceSize)
    : m_surfaceSize(surfaceSize)
{
}

bool EntityCreationSystem::initialize(flecs::world& world)
{
    world.entity()
        .set<blink::Position>({glm::vec3(0.0f, 1.0f, 3.0f)})
        .set<blink::CameraData>({glm::zero<glm::vec3>(), glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(45.0f), m_surfaceSize.x / m_surfaceSize.y, 0.1f, 10.0f});

    world.entity()
        .set<blink::Position>({glm::zero<glm::vec3>()})
        .set<blink::Rotation>({glm::identity<glm::quat>()})
        .set<blink::StaticModel>({"resource/monkey.gltf", "resource/materials/unlit.mat"});

    // auto e2 = world.entity();
    // e2.set<blink::Position>({glm::vec3(0.5f, 0.0f, 0.0f)});
    // e2.set<blink::Rotation>({glm::angleAxis(glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f))});
    // e2.set<blink::StaticModel>({m_geometry, m_wireframe });

    world.entity().set<blink::RenderFeature>({0, "resource/materials/wireframe.mat"});

    return true;
}
