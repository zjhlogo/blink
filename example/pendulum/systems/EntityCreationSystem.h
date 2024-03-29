/**

    @file      EntityCreationSystem.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <blink/components/Components.h>
#include <core/systems/ILogicalSystem.h>

class EntityCreationSystem final : public blink::ILogicalSystem
{
public:
    bool initialize() override;
    void terminate() override;

    void framePreUpdate() override;

    void renderLightPropertyUi();
    void renderMaterialPropertyUi();

private:
    blink::LightData m_lightData{glm::one<glm::vec3>(), 100.0f};
    bool m_lightDataDirty{};

    flecs::entity m_camera;
    flecs::entity m_light;
    //flecs::entity m_box;
    flecs::entity m_sphere;
    //flecs::entity m_tetrahedron;
};
