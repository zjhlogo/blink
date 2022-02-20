/**

    @file      EntityCreationSystem.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <core/base/ILogicalSystem.h>

class PrefabInitializeSystem;

class EntityCreationSystem : public blink::ILogicalSystem
{
public:
    EntityCreationSystem(PrefabInitializeSystem* prefabSystem, const glm::vec2& surfaceSize);

    virtual bool initialize(flecs::world& world) override;
    virtual void terminate(flecs::world& world) override;

public:
    flecs::entity m_camera;
    flecs::entity m_light;
    flecs::entity m_box;
    flecs::entity m_sphere;
    flecs::entity m_tetrahedron;

private:
    PrefabInitializeSystem* m_prefabSystem;
    glm::vec2 m_surfaceSize;

};
