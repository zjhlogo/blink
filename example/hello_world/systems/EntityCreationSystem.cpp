/**

    @file      EntityCreationSystem.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/

#include "EntityCreationSystem.h"
#include "PrefabInitializeSystem.h"

#include <blink/components/Components.h>
#include <blink/geometries/builder/BoxBuilder.h>
#include <blink/geometries/builder/PlaneBuilder.h>
#include <blink/geometries/builder/SphereUvBuilder.h>
#include <blink/geometries/builder/TetrahedronBuilder.h>
#include <blink/materials/Material.h>
#include <blink/resources/ResourceMgr.h>
#include <core/EcsWorld.h>
#include <core/components/Components.h>
#include <imgui/imgui.h>
#include <physics/components/Components.h>

EntityCreationSystem::EntityCreationSystem(const glm::vec2& surfaceSize)
    : m_surfaceSize(surfaceSize)
{
}

bool EntityCreationSystem::initialize()
{
    auto& world = m_ecsWorld->getWorld();
    auto prefabSystem = m_ecsWorld->findSystem<PrefabInitializeSystem>();

    // create camera
    m_camera = world.entity("camera");
    m_camera.set<blink::Position>({glm::vec3(0.0f, 0.0f, 4.0f)});
    m_camera.set<blink::Rotation>({glm::identity<glm::quat>()});
    m_camera.set<blink::CameraData>({glm::radians(45.0f), m_surfaceSize.x / m_surfaceSize.y, 0.1f, 10.0f});

    // light
    m_light = world.entity("light");
    m_light.set<blink::Position>({glm::vec3(0.0f, 0.0f, 4.0f)});
    m_light.set<blink::LightData>(m_lightData);

    //// load plane
    //{
    //    blink::PlaneBuilder builder;
    //    builder.size(20.0f, 20.0f);

    //    world.entity()
    //        .set<blink::Position>({glm::vec3(0.0f, 0.0f, 0.0f)})
    //        .set<blink::Rotation>({glm::identity<glm::quat>()})
    //        .set<blink::StaticModel>(
    //            {blink::ResourceMgr::getInstance().createGeometry(builder),
    //            blink::ResourceMgr::getInstance().createMaterial("resource/materials/unlit.mtl")});
    //}

    //// load mesh
    // world.entity()
    //    .set<blink::Position>({glm::vec3(0.0f, 0.0f, -1.0f)})
    //    .set<blink::Rotation>({glm::identity<glm::quat>()})
    //    .set<blink::AngularVelocity>({glm::vec3(0.0f, glm::radians(10.0f), 0.0f)})
    //    .set<blink::StaticModel>({blink::ResourceMgr::getInstance().createGeometry("resource/monkey.gltf"),
    //                              blink::ResourceMgr::getInstance().createMaterial("resource/materials/simple_lit.mtl")});

    // load box
    {
        m_box = world.entity("box").is_a(prefabSystem->prefabRigidBody);
        m_box.set<blink::Position>({glm::vec3(-1.5f, 0.0f, 0.0f)});

        blink::BoxBuilder builder;
        auto geometry = blink::ResourceMgr::getInstance().createGeometry(builder, true);
        auto material = blink::ResourceMgr::getInstance().createMaterial("resource/materials/wireframe.mtl");
        m_box.set<blink::StaticModel>({geometry, material});
        m_box.set<blink::PhysicsMass>(blink::PhysicsMass(10.0f, geometry->getInertiaTensor() * 10.0f));
    }

    // load sphere
    {
        m_sphere = world.entity("sphere").is_a(prefabSystem->prefabRigidBody);

        blink::SphereUvBuilder builder;
        auto geometry = blink::ResourceMgr::getInstance().createGeometry(builder, true);
        auto material = blink::ResourceMgr::getInstance().createMaterial("resource/materials/wireframe.mtl");
        m_sphere.set<blink::StaticModel>({geometry, material});
        m_sphere.set<blink::PhysicsMass>(blink::PhysicsMass(1.0f, geometry->getInertiaTensor()));
    }

    // load tetrahedron
    {
        m_tetrahedron = world.entity("tetrahedron").is_a(prefabSystem->prefabRigidBody);
        m_tetrahedron.set<blink::Position>({glm::vec3(1.5f, 0.0f, 0.0f)});

        blink::TetrahedronBuilder builder;
        auto geometry = blink::ResourceMgr::getInstance().createGeometry(builder, true);
        auto material = blink::ResourceMgr::getInstance().createMaterial("resource/materials/wireframe.mtl");
        m_tetrahedron.set<blink::StaticModel>({geometry, material});
        m_tetrahedron.set<blink::PhysicsMass>(blink::PhysicsMass(1.0f, geometry->getInertiaTensor()));
    }

    // auto e2 = world.entity();
    // e2.set<blink::Position>({glm::vec3(0.5f, 0.0f, 0.0f)});
    // e2.set<blink::Rotation>({glm::angleAxis(glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f))});
    // e2.set<blink::StaticModel>({m_geometry, m_wireframe });

    // world.entity().set<blink::RenderFeature>({0, "resource/materials/wireframe.mtl"});

    return true;
}

void EntityCreationSystem::terminate()
{
    //
}

void EntityCreationSystem::framePreUpdate()
{
    if (m_lightDataDirty)
    {
        m_lightDataDirty = false;
        m_light.set<blink::LightData>(m_lightData);
    }
}

void EntityCreationSystem::renderLightPropertyUi()
{
    if (ImGui::CollapsingHeader("light property", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::ColorEdit3("color1", (float*)&m_lightData.color))
        {
            m_lightDataDirty = true;
        }
        if (ImGui::SliderFloat("intensity", &m_lightData.intensity, 0.0f, 1000.0f, "intensity = %.3f"))
        {
            m_lightDataDirty = true;
        }
    }
}

void EntityCreationSystem::renderMaterialPropertyUi()
{
    if (ImGui::CollapsingHeader("material property", ImGuiTreeNodeFlags_DefaultOpen))
    {
        blink::Material* material = m_sphere.get<blink::StaticModel>()->material;
        auto roughness = material->getRoughness();
        if (ImGui::SliderFloat("roughness", &roughness, 0.0f, 1.0f, "roughness = %.3f"))
        {
            material->setRoughness(roughness);
        }
        auto metallic = material->getMetallic();
        if (ImGui::SliderFloat("metallic", &metallic, 0.0f, 1.0f, "metallic = %.3f"))
        {
            material->setMetallic(metallic);
        }
        glm::vec3 color = material->getColor();
        if (ImGui::ColorEdit3("color2", (float*)&color))
        {
            material->setColor(color);
        }
    }
}
