/**

    @file      EntityCreationSystem.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <blink/base/ISystemBase.h>
#include <blink/geometry/Geometry.h>
#include <blink/material/Material.h>

class EntityCreationSystem : public blink::ISystemBase
{
public:
    EntityCreationSystem(blink::Geometry* geometry, blink::Material* unlit, blink::Material* wireframe);

    virtual bool initialize(flecs::world& world) override;

private:
    blink::Geometry* m_geometry{};
    blink::Material* m_unlit{};
    blink::Material* m_wireframe{};
};
