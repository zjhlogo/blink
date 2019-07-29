#include "HelloWorldApp.h"
#include <geometry/BoxGeometry.h>
#include <material/PhongMaterial.h>
#include <light/PointLight.h>
#include <OpenGLRenderSystem.h>
#include <scene/SceneSystem.h>
#include <camera/CameraSystem.h>

entityx::Entity createBox(entityx::EntityManager& entities, std::shared_ptr<blink::BufferGeometry> geometry, std::shared_ptr<blink::Material> material, int id)
{
    static int s_currentId = 1;

    entityx::Entity box = entities.create();

    box.assign<blink::TransformData>();

    box.assign<blink::MeshData>(geometry, material);
    box.assign<blink::HierarchyData>(s_currentId++, id);

    return box;
}

bool HelloWorldApp::initialize()
{
    m_ex.systems.add<blink::OpenGLRenderSystem>();
    m_ex.systems.add<blink::CameraSystem>();
    m_ex.systems.add<blink::SceneSystem>();
    m_ex.systems.configure();

    // add cubes
    std::shared_ptr<blink::Material> material = std::make_shared<blink::PhongMaterial>();
    material->setTexture("tex_diffuse", "resource/grid16.png", 0);
    createBox(m_ex.entities, std::make_shared<blink::BoxGeometry>(1.0f, 1.0f, 1.0f), material, -1);

    // add light
    auto lightEntity = m_ex.entities.create();
    lightEntity.assign<blink::TransformData>()->position = glm::vec3(3.0f, 3.0f, 3.0f);
    lightEntity.assign<blink::LightData>(std::make_shared<blink::PointLight>());

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
    HelloWorldApp app;
    return blink::run(&app);
}
