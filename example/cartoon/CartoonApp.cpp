#include "CartoonApp.h"
#include "CartoonMaterial.h"
#include <Framework.h>
#include <geometries/SphereGeometry.h>
#include <geometries/BoxGeometry.h>
#include <lights/PointLight.h>
#include <render/RenderModule.h>
#include <cameras/TargetCamera.h>
#include <Log.h>

CartoonApp::CartoonApp()
    :IApp(1280, 720, "Cartoon")
{

}

CartoonApp::~CartoonApp()
{

}

bool CartoonApp::initialize()
{
    m_rootScene = new blink::Scene();

    auto material = new CartoonMaterial();
    material->setTexture("tex_cartoonLevel", "resource/cartoonlevel.png", 0);

    auto sphere = new blink::Mesh(new blink::SphereGeometry(0.5f), material);
    sphere->setPosition({ -1.0f, 0.0f, 0.0f });
    m_rootScene->add(sphere);

    auto cubeMesh = new blink::Mesh(new blink::BoxGeometry(1.0f, 1.0f, 1.0f), material);
    cubeMesh->setPosition({ 1.0f, 0.0f, 0.0f });
    m_rootScene->add(cubeMesh);

    auto light = new blink::PointLight();
    light->setPosition({ 2.0f, 2.0f, 2.0f });
    m_rootScene->add(light);

    auto camera = new blink::TargetCamera();
    camera->lookAt({ 0.0f, 0.0f, 3.0f }, blink::VEC3_ZERO, blink::VEC3_PY);
    m_camera = camera;

    return true;
}

void CartoonApp::terminate()
{
    SAFE_DELETE(m_camera);
    SAFE_DELETE(m_rootScene);
}

void CartoonApp::update(float dt)
{
    m_rootScene->updateWorldTransform(blink::MAT4_IDENTITY);
}

void CartoonApp::render()
{
    blink::RenderModule* pRenderModule = blink::Framework::getInstance().findComponent<blink::RenderModule>();
    if (!pRenderModule) return;

    pRenderModule->render(m_rootScene, m_camera);
}

int main(int argc, char** argv)
{
    return blink::Framework::getInstance().startup(new CartoonApp());
}
