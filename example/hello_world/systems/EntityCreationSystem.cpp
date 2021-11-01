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

EntityCreationSystem::EntityCreationSystem(NS::Mesh* mesh, NS::Material* material)
    : m_mesh(mesh)
    , m_material(material)
{
}

bool EntityCreationSystem::initialize(flecs::world& world)
{
    auto e1 = world.entity();
    e1.set<NS::Position>({ glm::zero<glm::vec3>() });
    e1.set<NS::Rotation>({ glm::identity<glm::quat>() });
    e1.set<NS::StaticModel>({ m_mesh, m_material });

    auto e2 = world.entity();
    e2.set<NS::Position>({ glm::vec3(0.5f, 0.0f, 0.0f) });
    e2.set<NS::Rotation>({ glm::angleAxis(glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) });
    e2.set<NS::StaticModel>({ m_mesh, m_material });

    return true;
}
