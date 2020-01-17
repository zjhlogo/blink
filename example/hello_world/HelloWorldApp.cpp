/*!
 * \file HelloWorldApp.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#include "HelloWorldApp.h"

#include <RenderData.h>
#include <camera/CameraSystem.h>
#include <geometry/BoxGeometry.h>
#include <light/PointLight.h>
#include <material/PhongMaterial.h>
#include <scene/SceneSystem.h>

entityx::Entity createBox(entityx::EntityManager& entities, NS::BufferGeometry* geometry, NS::Material* material, int id)
{
    static int s_currentId = 1;

    entityx::Entity box = entities.create();

    box.assign<NS::TransformData>();

    box.assign<NS::MeshData>(geometry, material);
    box.assign<NS::HierarchyData>(s_currentId++, id);

    return box;
}

HelloWorldApp::HelloWorldApp()
{
}

HelloWorldApp::~HelloWorldApp()
{
}

bool HelloWorldApp::initialize()
{
    m_ex.systems.add<NS::CameraSystem>();
    m_ex.systems.add<NS::SceneSystem>();
    m_ex.systems.configure();

    // add cubes
    NS::Material* material = new NS::PhongMaterial();
    material->setTexture("tex_diffuse", "resource/grid16.png", 0);
    createBox(m_ex.entities, new NS::BoxGeometry(1.0f, 1.0f, 1.0f), material, -1);

    // add light
    auto lightEntity = m_ex.entities.create();
    lightEntity.assign<NS::TransformData>()->position = glm::vec3(3.0f, 3.0f, 3.0f);
    lightEntity.assign<NS::LightData>(new NS::PointLight());

    // add a camera
    entityx::Entity camera = m_ex.entities.create();
    camera.assign<NS::CameraData>(glm::vec3(0.0f, 0.0f, 3.0f), NS::VEC3_ZERO, NS::VEC3_PY);

    return true;
}

void HelloWorldApp::terminate()
{
}

int main(int argc, char** argv)
{
    return NS::run("Vulkan");
}
