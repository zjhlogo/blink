/*********************************************************************
 * \file   LineArtEntityCreationSystem.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#pragma once

#include <blink/components/Components.h>
#include <core/ILogicalSystem.h>

class LineArtEntityCreationSystem final : public blink::ILogicalSystem
{
public:
    bool initialize() override;
    void terminate() override;
    void framePostUpdate() override;

    void renderMaterialPropertyUi();

private:
    blink::IMaterial* m_material{};
    glm::mat3 m_matViewToWorld{};
    glm::vec3 m_eyePos{4.0f, 2.0f, 4.0f};
    float m_fov{45.0f};
    glm::vec3 m_baseColor{1.0f, 0.0f, 0.0f};
    glm::vec3 m_lightPos{4.0f, 2.0f, 0.0f};
    glm::vec3 m_lightColor{0.4f, 0.4f, 0.4f};
    float m_lightIntensity{10.0f};
    float m_twist{3.0f};
};
