/*!
 * \file HelloWorldApp.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "HelloWorldApp.h"
#include <OpenGLRenderModule.h>
#include <geometry/BoxGeometry.h>
#include <material/PhongMaterial.h>
#include <light/PointLight.h>
#include <scene/SceneSystem.h>
#include <camera/CameraSystem.h>
#include <RenderData.h>

entityx::Entity createBox(entityx::EntityManager& entities, blink::BufferGeometry* geometry, blink::Material* material, int id)
{
    static int s_currentId = 1;

    entityx::Entity box = entities.create();

    box.assign<blink::TransformData>();

    box.assign<blink::MeshData>(geometry, material);
    box.assign<blink::HierarchyData>(s_currentId++, id);

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
    m_ex.systems.add<blink::CameraSystem>();
    m_ex.systems.add<blink::SceneSystem>();
    m_ex.systems.configure();

    // add cubes
    blink::Material* material = new blink::PhongMaterial();
    material->setTexture("tex_diffuse", "resource/grid16.png", 0);
    createBox(m_ex.entities, new blink::BoxGeometry(1.0f, 1.0f, 1.0f), material, -1);

    // add light
    auto lightEntity = m_ex.entities.create();
    lightEntity.assign<blink::TransformData>()->position = glm::vec3(3.0f, 3.0f, 3.0f);
    lightEntity.assign<blink::LightData>(new blink::PointLight());

    // add a camera
    entityx::Entity camera = m_ex.entities.create();
    camera.assign<blink::CameraData>(glm::vec3(0.0f, 0.0f, 3.0f), blink::VEC3_ZERO, blink::VEC3_PY);

    return true;
}

void HelloWorldApp::terminate()
{

}

int main(int argc, char** argv)
{
    blink::OpenGLRenderModule renderModule;
    return blink::run(&renderModule);
}
