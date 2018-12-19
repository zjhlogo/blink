#include "HelloWorldApp.h"
#include <Framework.h>
#include <render/geometries/BoxGeometry.h>
#include <render/materials/LamberMaterial.h>
#include <render/RenderModule.h>
#include <render/objects/Mesh.h>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char** argv)
{
    if (!blink::Framework::getInstance().initialize(new HelloWorldApp()))
    {
        blink::Framework::getInstance().terminate();
        return -1;
    }

	int result = blink::Framework::getInstance().start();
    blink::Framework::getInstance().terminate();

    return result;
}

HelloWorldApp::HelloWorldApp()
    :IApp(1280, 720, "Hello World")
{

}

HelloWorldApp::~HelloWorldApp()
{

}

bool HelloWorldApp::initialize()
{
    blink::Framework::getInstance().insertComponent(new blink::RenderModule());

    m_rootScene = new blink::Scene();

    m_cube = new blink::Mesh(new blink::BoxGeometry(1.0f, 1.0f, 1.0f), new blink::LamberMaterial());
    m_rootScene->add(m_cube);

    m_camera = new blink::PerspectiveCamera();
    m_camera->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    m_camera->setTargetPosition(blink::VEC3_ZERO);

    return true;
}

void HelloWorldApp::terminate()
{
    SAFE_DELETE(m_camera);
    SAFE_DELETE(m_rootScene);
}

void HelloWorldApp::update(float dt)
{
    static float s_totalTime = 0.0f;
    s_totalTime += dt;
    m_cube->setRotation(glm::angleAxis(glm::radians(45.0f) * s_totalTime, glm::normalize(glm::vec3(0.5f, 1.0f, 0.5f))));
    m_rootScene->updateWorldTransform(blink::MAT4_IDENTITY);
}

void HelloWorldApp::render()
{
    blink::RenderModule* pRenderModule = blink::Framework::getInstance().findComponent<blink::RenderModule>();
    if (!pRenderModule) return;

    pRenderModule->renderObject(m_cube, m_camera);
}
