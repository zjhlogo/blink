#include "HelloWorldApp.h"
#include <render/OpenGL3RenderSystem.h>
#include <scene/SceneSystem.h>
#include <camera/CameraSystem.h>
#include <input/InputSystem.h>
#include <render/GeometryBuilder.h>

entityx::Entity createBox(entityx::EntityManager& entities, float width, float height, float depth, entityx::Entity& parentEntity)
{
    static int s_currentId = 1;

    entityx::Entity box = entities.create();

    box.assign<blink::Transform>();

    auto geometry = box.assign<blink::BufferGeometry>();
    blink::GeometryBuilder::buildBox(*geometry.get(), width, height, depth);

    box.assign<blink::PhongMaterial>();

    if (parentEntity.valid() && parentEntity.has_component<blink::Hierarchy>())
    {
        box.assign<blink::Hierarchy>(s_currentId++, parentEntity.component<blink::Hierarchy>()->id);
    }
    else
    {
        box.assign<blink::Hierarchy>(s_currentId++, -1);
    }

    return box;
}

bool HelloWorldApp::initialize()
{
    auto cameraSystem = m_ex.systems.add<blink::CameraSystem>();
    m_ex.systems.add<blink::SceneSystem>();
    auto renderSystem = m_ex.systems.add<blink::OpenGL3RenderSystem>();
    m_ex.systems.add<blink::InputSystem>();
    m_ex.systems.configure();

    // add cubes
    entityx::Entity cube = createBox(m_ex.entities, 1.0f, 1.0f, 1.0f, entityx::Entity());

    // add a camera
    entityx::Entity camera = m_ex.entities.create();
    camera.assign<blink::CameraData>(glm::vec3(0.0f, 0.0f, 3.0f), blink::VEC3_ZERO, blink::VEC3_PY);

    cameraSystem->setCamera(camera);
    renderSystem->setCamera(camera);

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
