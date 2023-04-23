/*********************************************************************
 * \file   PhysicsApp.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#include "PhysicsApp.h"
#include "blink/components/Components.h"
#include "blink/geometries_builder/BoxBuilder.h"
#include "blink/geometries_builder/SphereUvBuilder.h"
#include "common/render_systems/ImguiRenderSystem.h"
#include "common/utils/SceneEntityUtil.h"
#include "core/components/Components.h"
#include "core/modules/IResourceModule.h"
#include "imgui/imgui.h"

#include <blink/blink.h>
#include <physics/JoltPhysicsSystem.h>

bool PhysicsApp::initializeLogicalSystems()
{
    SceneEntityUtil::initializeCommonLogicalSystems(this);

    m_physicsSystem = addLogicSystem(new blink::JoltPhysicsSystem());

    return true;
}

bool PhysicsApp::initializeRenderSystems()
{
    auto guiRenderSystem = addRenderSystem(new ImguiRenderSystem());
    guiRenderSystem->addWindow(this);

    return true;
}

bool PhysicsApp::startup()
{
    if (!SceneEntityUtil::initializeCommonSceneEntities(getEcsWorld()))
    {
        return false;
    }

    auto& world = getEcsWorld().getWorld();
    auto resourceModule = blink::getResourceModule();

    // ground
    {
        auto mtlSimpleLit = resourceModule->createMaterial("/materials/simple_lit.mtl");
        blink::BoxBuilder builder;
        auto boxSize = glm::vec3(100, 1, 100);
        auto geometry = builder.size(boxSize).build();
        auto pos = glm::zero<glm::vec3>();
        auto rot = glm::identity<glm::quat>();
        auto scale = glm::one<glm::vec3>();
        auto bodyId = m_physicsSystem->CreateBox(boxSize, pos, rot, blink::PhysicsBodyType::Static);
        auto entity = world.entity("ground");
        entity.set<blink::Position>({pos});
        entity.set<blink::Rotation>({rot});
        entity.set<blink::Scale>({scale});
        entity.set<blink::StaticModel>({geometry, mtlSimpleLit});
        entity.set<blink::Renderable>({blink::RenderLayers::NORMAL});
        entity.set<blink::PhysicsData>({bodyId});
    }

    // sphere
    {
        auto mtlSimpleLit = resourceModule->createMaterial("/materials/simple_lit.mtl");
        blink::SphereUvBuilder builder;
        auto radius = 0.05f;
        auto geometry = builder.radius(radius).build();
        auto pos = glm::vec3(0.0f, 1.0f, 0.0f);
        auto rot = glm::identity<glm::quat>();
        auto scale = glm::one<glm::vec3>();
        auto bodyId = m_physicsSystem->CreateSphere(radius, pos, rot, blink::PhysicsBodyType::Dynamic);
        auto entity = world.entity("sphere");
        entity.set<blink::Position>({pos});
        entity.set<blink::Rotation>({rot});
        entity.set<blink::Scale>({scale});
        entity.set<blink::StaticModel>({geometry, mtlSimpleLit});
        entity.set<blink::Renderable>({blink::RenderLayers::NORMAL});
        entity.set<blink::PhysicsData>({bodyId});
    }

    return true;
}

void PhysicsApp::onGui()
{
    ImGui::Begin("Test");
    ImGui::End();
}

int main(int argc, char** argv)
{
    PhysicsApp app;
    return blink::run(argc, argv, app);
}
