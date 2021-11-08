/**

    @file      EntityCreationSystem.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/

#include "EntityCreationSystem.h"

#include <blink/component/Components.h>
#include <blink/geometry/builder/BoxBuilder.h>
#include <blink/geometry/builder/PlaneBuilder.h>
#include <blink/geometry/builder/SphereUvBuilder.h>
#include <blink/geometry/builder/TetrahedronBuilder.h>
#include <blink/resource/ResourceMgr.h>

EntityCreationSystem::EntityCreationSystem(const glm::vec2& surfaceSize)
    : m_surfaceSize(surfaceSize)
{
}

bool EntityCreationSystem::initialize(flecs::world& world)
{
    // create camera
    world.entity()
        .set<blink::Position>({glm::vec3(0.0f, 1.0f, 8.0f)})
        .set<blink::Rotation>({glm::angleAxis(glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f))})
        .set<blink::CameraData>({glm::radians(45.0f), m_surfaceSize.x / m_surfaceSize.y, 0.1f, 10.0f});

    // load plane
    {
        blink::PlaneBuilder builder;
        builder.size(20.0f, 20.0f);

        world.entity()
            .set<blink::Position>({glm::vec3(0.0f, 0.0f, 0.0f)})
            .set<blink::Rotation>({glm::identity<glm::quat>()})
            .set<blink::StaticModel>(
                {blink::ResourceMgr::getInstance().createGeometry(builder), blink::ResourceMgr::getInstance().createMaterial("resource/materials/unlit.mtl")});
    }

    // load mesh
    world.entity()
        .set<blink::Position>({glm::vec3(0.0f, 1.0f, 0.0f)})
        .set<blink::Rotation>({glm::identity<glm::quat>()})
        .set<blink::AngularVelocity>({glm::vec3(0.0f, glm::radians(90.0f), 0.0f)})
        .set<blink::StaticModel>({blink::ResourceMgr::getInstance().createGeometry("resource/monkey.gltf"),
                                  blink::ResourceMgr::getInstance().createMaterial("resource/materials/simple_lit.mtl")});

    // load box
    {
        blink::BoxBuilder builder;

        world.entity()
            .set<blink::Position>({glm::vec3(-2.0f, 1.0f, 0.0f)})
            .set<blink::Rotation>({glm::identity<glm::quat>()})
            .set<blink::AngularVelocity>({glm::vec3(0.0f, glm::radians(80.0f), 0.0f)})
            .set<blink::StaticModel>({blink::ResourceMgr::getInstance().createGeometry(builder),
                                      blink::ResourceMgr::getInstance().createMaterial("resource/materials/simple_lit.mtl")});
    }

    // load sphere
    {
        blink::SphereUvBuilder builder;

        world.entity()
            .set<blink::Position>({glm::vec3(2.0f, 1.0f, 0.0f)})
            .set<blink::Rotation>({glm::identity<glm::quat>()})
            .set<blink::AngularVelocity>({glm::vec3(0.0f, glm::radians(70.0f), 0.0f)})
            .set<blink::StaticModel>({blink::ResourceMgr::getInstance().createGeometry(builder),
                                      blink::ResourceMgr::getInstance().createMaterial("resource/materials/simple_lit.mtl")});
    }

    // load tetrahedron
    {
        blink::TetrahedronBuilder builder;

        world.entity()
            .set<blink::Position>({glm::vec3(-4.0f, 1.0f, 0.0f)})
            .set<blink::Rotation>({glm::identity<glm::quat>()})
            .set<blink::AngularVelocity>({glm::vec3(0.0f, glm::radians(60.0f), 0.0f)})
            .set<blink::StaticModel>({blink::ResourceMgr::getInstance().createGeometry(builder),
                                      blink::ResourceMgr::getInstance().createMaterial("resource/materials/simple_lit.mtl")});
    }

    // auto e2 = world.entity();
    // e2.set<blink::Position>({glm::vec3(0.5f, 0.0f, 0.0f)});
    // e2.set<blink::Rotation>({glm::angleAxis(glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f))});
    // e2.set<blink::StaticModel>({m_geometry, m_wireframe });

    // world.entity().set<blink::RenderFeature>({0, "resource/materials/wireframe.mtl"});

    return true;
}
