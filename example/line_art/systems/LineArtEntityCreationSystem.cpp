
/*********************************************************************
 * \file   LineArtEntityCreationSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#include "LineArtEntityCreationSystem.h"

#include <blink/geometries_builder/PlaneBuilder.h>
#include <core/components/Components.h>
#include <core/modules/IRenderModule.h>
#include <core/modules/IResModule.h>
#include <imgui/imgui.h>

bool LineArtEntityCreationSystem::initialize()
{
    auto& world = m_ecsWorld->getWorld();
    auto resModule = blink::getResModule();
    auto renderModule = blink::getRenderModule();
    const auto& surfaceSize = renderModule->getSurfaceSize();

    // create camera
    {
        auto halfSurfaceSize = surfaceSize * 0.5f;
        auto camera = world.entity("camera");
        camera.set<blink::Position>({glm::vec3(0.0f, 0.0f, 100.0f)});
        camera.set<blink::Rotation>({glm::quatLookAt(glm::forward(), glm::up())});
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
                            .orient(glm::angleAxis(glm::half_pi<float>(), glm::right()))
                            .build(false, false);
        m_material = resModule->createMaterial("resource/materials/sdf.mtl");
        m_material->setUniform("fov", glm::radians(m_fov));
        plane.set<blink::StaticModel>({geometry, m_material});
    }

    world.entity().set<blink::RenderFeature>({blink::RenderOrders::DYNAMIC_OPAQUE, blink::RenderLayers::ALL, nullptr});

    return true;
}

void LineArtEntityCreationSystem::terminate()
{
    //
}

void LineArtEntityCreationSystem::framePostUpdate()
{
    auto& world = m_ecsWorld->getWorld();
    auto matRot4 = glm::rotate(glm::identity<glm::mat4>(), 1.0f * world.time(), glm::up());
    m_material->setUniform("matRotate", glm::mat3(matRot4));
}

void LineArtEntityCreationSystem::renderMaterialPropertyUi()
{
    if (ImGui::CollapsingHeader("material property", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::SliderFloat("fov", &m_fov, 0.0f, 180.0f))
        {
            m_material->setUniform("fov", glm::radians(m_fov));
        }
    }
}
