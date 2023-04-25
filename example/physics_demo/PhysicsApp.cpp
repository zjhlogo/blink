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
    createSandbox(100.0f, 4.0f, 100.0f, 2.0f);

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

void PhysicsApp::createSandbox(float width, float height, float depth, float thick)
{
    auto& world = getEcsWorld().getWorld();
    auto resourceModule = blink::getResourceModule();
    auto mtlSimpleLit = resourceModule->createMaterial("/materials/simple_lit.mtl");

    // bottom
    {
        blink::BoxBuilder builder;
        glm::vec3 geoSize(width + thick * 2.0f, thick, depth + thick * 2.0f);
        auto geometry = builder.size(geoSize).build();
        glm::vec3 pos(0.0f, -0.5f * thick, 0.0f);
        auto rot = glm::identity<glm::quat>();
        auto scale = glm::one<glm::vec3>();
        auto bodyId = m_physicsSystem->CreateBox(geoSize, pos, rot, blink::PhysicsBodyType::Static);
        auto entity = world.entity();
        entity.set<blink::Position>({pos});
        entity.set<blink::Rotation>({rot});
        entity.set<blink::Scale>({scale});
        entity.set<blink::StaticModel>({geometry, mtlSimpleLit});
        entity.set<blink::Renderable>({blink::RenderLayers::NORMAL});
        entity.set<blink::PhysicsData>({bodyId});
    }

    // left
    {
        blink::BoxBuilder builder;
        glm::vec3 geoSize(thick, thick + height, depth + thick * 2.0f);
        auto geometry = builder.size(geoSize).build();
        glm::vec3 pos(-0.5f * (width + thick), 0.5f * (height - thick), 0.0f);
        auto rot = glm::identity<glm::quat>();
        auto scale = glm::one<glm::vec3>();
        auto bodyId = m_physicsSystem->CreateBox(geoSize, pos, rot, blink::PhysicsBodyType::Static);
        auto entity = world.entity();
        entity.set<blink::Position>({pos});
        entity.set<blink::Rotation>({rot});
        entity.set<blink::Scale>({scale});
        entity.set<blink::StaticModel>({geometry, mtlSimpleLit});
        entity.set<blink::Renderable>({blink::RenderLayers::NORMAL});
        entity.set<blink::PhysicsData>({bodyId});
    }

    // right
    {
        blink::BoxBuilder builder;
        glm::vec3 geoSize(thick, thick + height, depth + thick * 2.0f);
        auto geometry = builder.size(geoSize).build();
        glm::vec3 pos(0.5f * (width + thick), 0.5f * (height - thick), 0.0f);
        auto rot = glm::identity<glm::quat>();
        auto scale = glm::one<glm::vec3>();
        auto bodyId = m_physicsSystem->CreateBox(geoSize, pos, rot, blink::PhysicsBodyType::Static);
        auto entity = world.entity();
        entity.set<blink::Position>({pos});
        entity.set<blink::Rotation>({rot});
        entity.set<blink::Scale>({scale});
        entity.set<blink::StaticModel>({geometry, mtlSimpleLit});
        entity.set<blink::Renderable>({blink::RenderLayers::NORMAL});
        entity.set<blink::PhysicsData>({bodyId});
    }

    // front
    {
        blink::BoxBuilder builder;
        glm::vec3 geoSize(width + thick * 2.0f, thick + height, thick);
        auto geometry = builder.size(geoSize).build();
        glm::vec3 pos(0.0f, 0.5f * (height - thick), 0.5f * (depth + thick));
        auto rot = glm::identity<glm::quat>();
        auto scale = glm::one<glm::vec3>();
        auto bodyId = m_physicsSystem->CreateBox(geoSize, pos, rot, blink::PhysicsBodyType::Static);
        auto entity = world.entity();
        entity.set<blink::Position>({pos});
        entity.set<blink::Rotation>({rot});
        entity.set<blink::Scale>({scale});
        entity.set<blink::StaticModel>({geometry, mtlSimpleLit});
        entity.set<blink::Renderable>({blink::RenderLayers::NORMAL});
        entity.set<blink::PhysicsData>({bodyId});
    }

    // back
    {
        blink::BoxBuilder builder;
        glm::vec3 geoSize(width + thick * 2.0f, thick + height, thick);
        auto geometry = builder.size(geoSize).build();
        glm::vec3 pos(0.0f, 0.5f * (height - thick), -0.5f * (depth + thick));
        auto rot = glm::identity<glm::quat>();
        auto scale = glm::one<glm::vec3>();
        auto bodyId = m_physicsSystem->CreateBox(geoSize, pos, rot, blink::PhysicsBodyType::Static);
        auto entity = world.entity();
        entity.set<blink::Position>({pos});
        entity.set<blink::Rotation>({rot});
        entity.set<blink::Scale>({scale});
        entity.set<blink::StaticModel>({geometry, mtlSimpleLit});
        entity.set<blink::Renderable>({blink::RenderLayers::NORMAL});
        entity.set<blink::PhysicsData>({bodyId});
    }
}

int main(int argc, char** argv)
{
    PhysicsApp app;
    return blink::run(argc, argv, app);
}
