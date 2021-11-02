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
#include <blink/geometry/Mesh.h>
#include <blink/material/Material.h>

class EntityCreationSystem : public blink::ISystemBase
{
public:
    EntityCreationSystem(blink::IGeometry* geometry, blink::Material* material);

    virtual bool initialize(flecs::world& world) override;

private:
    blink::IGeometry* m_geometry{};
    blink::Material* m_material{};
};
