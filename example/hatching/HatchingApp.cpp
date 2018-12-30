#include "HatchingApp.h"
#include "HatchingMaterial.h"
#include <Framework.h>
#include <geometries/BoxGeometry.h>
#include <lights/AmbientLight.h>
#include <lights/PointLight.h>
#include <render/RenderModule.h>

HatchingApp::HatchingApp()
    :IApp(1280, 720, "Hello World")
{

}

HatchingApp::~HatchingApp()
{

}

bool HatchingApp::initialize()
{
    m_rootScene = new blink::Scene();

    auto material = new HatchingMaterial();
    material->setTexture("tex_diffuse", "resource/hatching.png", 0);
    material->setTexture("tex_hatchingLevel", "resource/hatchinglevel.png", 1);
    m_sphere = new blink::Mesh(new blink::BoxGeometry(1.0f, 1.0f, 1.0f), material);
    m_rootScene->add(m_sphere);

    m_rootScene->add(new blink::AmbientLight());

    blink::PointLight* light = new blink::PointLight();
    light->setPosition({ 0.0f, 5.0f, 1.0f });
    m_rootScene->add(light);

    m_camera = new blink::PerspectiveCamera();
    m_camera->setPosition({ 0.0f, 3.0f, 3.0f });
    m_camera->setTargetPosition(blink::VEC3_ZERO);

    return true;
}

void HatchingApp::terminate()
{
    SAFE_DELETE(m_camera);
    SAFE_DELETE(m_rootScene);
}

void HatchingApp::update(float dt)
{
    m_sphere->applyRotation(4.9f * dt, blink::VEC3_PX);
    m_sphere->applyRotation(6.9f * dt, blink::VEC3_PY);
    m_rootScene->updateWorldTransform(blink::MAT4_IDENTITY);
}

void HatchingApp::render()
{
    blink::RenderModule* pRenderModule = blink::Framework::getInstance().findComponent<blink::RenderModule>();
    if (!pRenderModule) return;

    pRenderModule->render(m_rootScene, m_camera);
}

int main(int argc, char** argv)
{
    return blink::Framework::getInstance().startup(new HatchingApp());
}
