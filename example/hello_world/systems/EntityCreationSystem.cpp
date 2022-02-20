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

#include <blink/component/Components.h>
#include <blink/geometry/builder/BoxBuilder.h>
#include <blink/geometry/builder/PlaneBuilder.h>
#include <blink/geometry/builder/SphereUvBuilder.h>
#include <blink/geometry/builder/TetrahedronBuilder.h>
#include <blink/resource/ResourceMgr.h>
#include <core/component/Components.h>
#include <physics/component/Components.h>

EntityCreationSystem::EntityCreationSystem(PrefabInitializeSystem* prefabSystem, const glm::vec2& surfaceSize)
    : m_prefabSystem(prefabSystem)
    , m_surfaceSize(surfaceSize)
{
}

bool EntityCreationSystem::initialize(flecs::world& world)
{
    // create camera
    m_camera = world.entity();
    m_camera.set<blink::Position>({glm::vec3(0.0f, 0.0f, 4.0f)});
    m_camera.set<blink::Rotation>({glm::identity<glm::quat>()});
    m_camera.set<blink::CameraData>({glm::radians(45.0f), m_surfaceSize.x / m_surfaceSize.y, 0.1f, 10.0f});

    // light
    m_light = world.entity();
    m_light.set<blink::Position>({glm::vec3(0.0f, 0.0f, 4.0f)});
    m_light.set<blink::LightData>({glm::one<glm::vec3>(), 100.0f});

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

    //// load box
    //{
    //    m_box = world.entity();
    //    m_box.set<blink::Position{glm::vec3(-1.0f, 0.0f, 0.0f)});
    //    m_box.set<blink::Rotation{glm::identity<glm::quat>()});
    //    m_box.set<blink::AngularVelocity{glm::vec3(0.0f, glm::radians(80.0f), 0.0f)});

    //    blink::BoxBuilder builder;
    //    auto geometry = blink::ResourceMgr::getInstance().createGeometry(builder);
    //    auto material = blink::ResourceMgr::getInstance().createMaterial("resource/materials/unlit.mtl");
    //    m_box.set<blink::StaticModel{geometry, material});
    //}

    // load sphere
    {
        m_sphere = world.entity().is_a(m_prefabSystem->prefabRigidBody);
        m_sphere.set<blink::PhysicsAccumulate>({glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(50.5f, 1.0f, 3.0f)});
        blink::BoxBuilder builder;
        builder.size(0.5f, 0.5f, 0.5f);
        auto geometry = blink::ResourceMgr::getInstance().createGeometry(builder);
        auto material = blink::ResourceMgr::getInstance().createMaterial("resource/materials/pbr_lit.mtl");
        m_sphere.set<blink::StaticModel>({geometry, material});
    }

    //// load tetrahedron
    //{
    //    m_tetrahedron = world.entity();
    //    m_tetrahedron.set<blink::Position>({glm::vec3(1.0f, 0.0f, 0.0f)});
    //    m_tetrahedron.set<blink::Rotation>({glm::identity<glm::quat>()});
    //    m_tetrahedron.set<blink::AngularVelocity>({glm::vec3(0.0f, glm::radians(60.0f), 0.0f)});

    //    blink::TetrahedronBuilder builder;
    //    auto geometry = blink::ResourceMgr::getInstance().createGeometry(builder);
    //    auto material = blink::ResourceMgr::getInstance().createMaterial("resource/materials/wireframe.mtl");
    //    m_tetrahedron.set<blink::StaticModel>({geometry, material});
    //}

    // auto e2 = world.entity();
    // e2.set<blink::Position>({glm::vec3(0.5f, 0.0f, 0.0f)});
    // e2.set<blink::Rotation>({glm::angleAxis(glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f))});
    // e2.set<blink::StaticModel>({m_geometry, m_wireframe });

    // world.entity().set<blink::RenderFeature>({0, "resource/materials/wireframe.mtl"});

    return true;
}

void EntityCreationSystem::terminate(flecs::world& world)
{
    //
}
