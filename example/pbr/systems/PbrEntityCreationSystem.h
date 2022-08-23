
/*********************************************************************
 * \file   PbrEntityCreationSystem.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/

#pragma once

#include <blink/components/Components.h>
#include <core/ILogicalSystem.h>

class PbrEntityCreationSystem : public blink::ILogicalSystem
{
public:
    virtual bool initialize() override;
    virtual void terminate() override;
    virtual void framePostUpdate() override;

    void renderMaterialPropertyUi();

private:
    blink::IMaterial* m_material{};
    glm::mat3 m_matViewToWorld;
    glm::vec3 m_eyePos{4.0f, 2.0f, 4.0f};
    float m_fov{45.0f};
    glm::vec3 m_baseColor{1.0f, 0.0f, 0.0f};
    glm::vec3 m_lightPos{4.0f, 2.0f, 0.0f};
    glm::vec3 m_lightColor{0.4f, 0.4f, 0.4f};
    float m_lightIntensity{10.0f};
    float m_twist{3.0f};
};
