#include "HelloWorldApp.h"
#include <systems/OpenGL3RenderSystem.h>
#include <systems/SceneSystem.h>
#include <Components.h>
#include <Materials.h>
#include <GeometryBuilder.h>

bool HelloWorldApp::initialize()
{
    auto scene = m_ex.systems.add<blink::SceneSystem>();
    m_ex.systems.add<blink::OpenGL3RenderSystem>();
    m_ex.systems.configure();

    entityx::Entity cube = m_ex.entities.create();

    cube.assign<blink::Transform>();

    auto geometry = cube.assign<blink::BufferGeometry>();
    blink::GeometryBuilder::buildBox(*geometry.get(), 1.0f, 1.0f, 1.0f);

    auto material = cube.assign<blink::PhongMaterial>();

    scene->add(cube, nullptr);

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
