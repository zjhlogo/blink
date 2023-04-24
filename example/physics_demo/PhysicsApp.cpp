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

#include <random>

bool PhysicsApp::initializeLogicalSystems()
{
    SceneEntityUtil::initializeCommonLogicalSystems(this);

    m_physicsSystem = addLogicSystem(new blink::JoltPhysicsSystem(true));

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

    auto mtlSimpleLit = resourceModule->createMaterial("/materials/simple_lit.mtl");

    // ground
    {
        blink::BoxBuilder builder;
        auto boxSize = glm::vec3(1000.0f, 1.0f, 1000.0f);
        auto geometry = builder.size(boxSize).build();
        auto pos = glm::vec3(0.0f, -0.5f, 0.0f);
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
    std::default_random_engine re;
    re.seed(9999);
//    for (int i = 0; i < 100; ++i)
//    {
//        blink::SphereUvBuilder builder;
//        auto radius = blink::randomRange(re, 0.5f, 1.0f);
//        auto geometry = builder.radius(radius).build();
//        auto pos = glm::vec3(blink::randomRange(re, -10.0f, 10.0f),
//                             blink::randomRange(re, 10.0f, 30.0f),
//                             blink::randomRange(re, -10.0f, 10.0f));
//        auto rot = glm::identity<glm::quat>();
//        auto scale = glm::one<glm::vec3>();
//        auto bodyId = m_physicsSystem->CreateSphere(radius, pos, rot, blink::PhysicsBodyType::Dynamic);
//        auto entity = world.entity();
//        entity.set<blink::Position>({pos});
//        entity.set<blink::Rotation>({rot});
//        entity.set<blink::Scale>({scale});
//        entity.set<blink::StaticModel>({geometry, mtlSimpleLit});
//        entity.set<blink::Renderable>({blink::RenderLayers::NORMAL});
//        entity.set<blink::PhysicsData>({bodyId});
//    }

    for (int i = 0; i < 100; ++i)
    {
        blink::BoxBuilder builder;
        auto size = blink::randomRange(re, 0.5f, 2.0f);
        auto boxSize = glm::vec3(size, size, size);
        auto geometry = builder.size(boxSize).build();
        auto pos = glm::vec3(blink::randomRange(re, -10.0f, 10.0f),
                             blink::randomRange(re, 10.0f, 30.0f),
                             blink::randomRange(re, -10.0f, 10.0f));
        auto rot = blink::randomQuat(re);
        auto scale = glm::one<glm::vec3>();
        auto bodyId = m_physicsSystem->CreateBox(boxSize, pos, rot, blink::PhysicsBodyType::Dynamic);
        auto entity = world.entity();
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
    ImGui::Begin("Physics World Info");

    auto tick = m_physicsSystem->getFrameTick();
    ImGui::InputScalar("Frame Tick", ImGuiDataType_U32, &tick);

    auto hash = m_physicsSystem->getFrameHash();
    ImGui::InputScalar("Frame Hash", ImGuiDataType_U32, &hash);

    if (m_physicsSystem->isPause())
    {
        if (ImGui::Button("Play"))
        {
            m_physicsSystem->setPause(false);
        }
    }
    else
    {
        if (ImGui::Button("Pause"))
        {
            m_physicsSystem->setPause(true);
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Step"))
    {
        m_physicsSystem->setPause(false);
        m_physicsSystem->setPauseFrameTick(m_physicsSystem->getFrameTick() + 1);
    }

    if (ImGui::Button("Save State"))
    {
        // save state
        m_physicsSystem->saveState("state.bin");
    }

    ImGui::SameLine();
    if (ImGui::Button("Load State"))
    {
        // load state
        m_physicsSystem->loadState("state.bin");
    }

    ImGui::End();
}

int main(int argc, char** argv)
{
    PhysicsApp app;
    return blink::run(argc, argv, app);
}
