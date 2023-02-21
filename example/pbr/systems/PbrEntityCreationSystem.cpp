/*********************************************************************
 * \file   PbrEntityCreationSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#include "PbrEntityCreationSystem.h"

#include <blink/geometries_builder/PlaneBuilder.h>
#include <core/components/Components.h>
#include <core/modules/IRenderModule.h>
#include <core/modules/IResourceModule.h>
#include <imgui/imgui.h>

bool PbrEntityCreationSystem::initialize()
{
    auto& world = m_ecsWorld->getWorld();
    auto resModule = blink::getResourceModule();
    auto renderModule = blink::getRenderModule();
    const auto& surfaceSize = renderModule->getSurfaceSize();

    // create camera
    {
        auto halfSurfaceSize = surfaceSize * 0.5f;
        auto camera = world.entity("camera");
        camera.set<blink::Position>({glm::vec3(0.0f, 0.0f, 100.0f)});
        camera.set<blink::Rotation>({quatLookAt(glm::forward(), glm::up())});
        camera.set<blink::CameraData>(
            {glm::ortho(-halfSurfaceSize.x, halfSurfaceSize.x, -halfSurfaceSize.y, halfSurfaceSize.y, -1000.0f, 1000.0f)});
    }

    // create a plane
    {
        auto plane = world.entity("plane");
        plane.set<blink::Position>({glm::zero<glm::vec3>()});
        plane.set<blink::Rotation>({glm::identity<glm::quat>()});
        plane.set<blink::Renderable>({blink::RenderLayers::NORMAL});

        blink::PlaneBuilder builder;
        auto geometry = builder.size(surfaceSize.x, surfaceSize.y)
                            .orient(angleAxis(glm::half_pi<float>(), glm::right()))
                            .build(false, false);
        m_material = resModule->createMaterial("resource/materials/sdf.mtl");

        m_matViewToWorld = inverse(lookAt(m_eyePos, glm::zero<glm::vec3>(), glm::up()));
        m_material->setUniform("matViewToWorld", m_matViewToWorld);
        m_material->setUniform("fov", glm::radians(m_fov));
        m_material->setUniform("eyePos", m_eyePos);
        m_material->setUniform("baseColor", m_baseColor);
        m_material->setUniform("lightPos", m_lightPos);
        m_material->setUniform("lightColor", m_lightColor);
        m_material->setUniform("lightIntensity", m_lightIntensity);
        m_material->setUniform("twist", m_twist);

        plane.set<blink::StaticModel>({geometry, m_material});
    }

    world.entity().set<blink::RenderFeature>({blink::RenderOrders::DYNAMIC_OPAQUE, blink::RenderLayers::ALL, nullptr});

    return true;
}

void PbrEntityCreationSystem::terminate()
{
    //
}

void PbrEntityCreationSystem::framePostUpdate()
{
    auto& world = m_ecsWorld->getWorld();
    m_lightPos.x = 4.0f * glm::sin(world.time());
    m_lightPos.z = 4.0f * glm::cos(world.time());
    m_material->setUniform("lightPos", m_lightPos);
}

void PbrEntityCreationSystem::renderMaterialPropertyUi()
{
    if (ImGui::CollapsingHeader("material property", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("eyePos", &m_eyePos[0], 0.1f))
        {
            m_material->setUniform("eyePos", m_eyePos);
            m_matViewToWorld = inverse(lookAt(m_eyePos, glm::zero<glm::vec3>(), glm::up()));
            m_material->setUniform("matViewToWorld", m_matViewToWorld);
        }

        if (ImGui::SliderFloat("fov", &m_fov, 0.0f, 180.0f))
        {
            m_material->setUniform("fov", glm::radians(m_fov));
        }

        if (ImGui::ColorEdit3("baseColor", &m_baseColor[0]))
        {
            m_material->setUniform("baseColor", m_baseColor);
        }

        if (ImGui::ColorEdit3("lightColor", &m_lightColor[0]))
        {
            m_material->setUniform("lightColor", m_lightColor);
        }

        if (ImGui::SliderFloat("lightIntensity", &m_lightIntensity, 1.0f, 100.0f))
        {
            m_material->setUniform("lightIntensity", m_lightIntensity);
        }

        if (ImGui::SliderFloat("twist", &m_twist, -10.0f, 10.0f))
        {
            m_material->setUniform("twist", m_twist);
        }
    }
}
