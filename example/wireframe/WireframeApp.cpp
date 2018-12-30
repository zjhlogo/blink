#include "WireframeApp.h"
#include <Framework.h>
#include <geometries/SphereGeometry.h>
#include <geometries/BoxGeometry.h>
#include <materials/WireframeMaterial.h>
#include <render/RenderModule.h>

WireframeApp::WireframeApp()
    :IApp(1280, 720, "Wireframe")
{

}

WireframeApp::~WireframeApp()
{

}

bool WireframeApp::initialize()
{
    m_rootScene = new blink::Scene();

    auto wireframeMaterial = new blink::WireframeMaterial();

    // add sphere
    m_sphere = new blink::Mesh(new blink::SphereGeometry(0.5f, 8, 15), wireframeMaterial);
    m_sphere->setPosition({ -1.0f, 0.0f, 0.0f });
    m_rootScene->add(m_sphere);

    // add cube
    m_cube = new blink::Mesh(new blink::BoxGeometry(1.0f, 1.0f, 1.0f), wireframeMaterial);
    m_cube->setPosition({ 1.0f, 0.0f, 0.0f });
    m_rootScene->add(m_cube);

    m_camera = new blink::PerspectiveCamera();
    m_camera->setPosition({ 0.0f, 3.0f, 3.0f });
    m_camera->setTargetPosition(blink::VEC3_ZERO);

    return true;
}

void WireframeApp::terminate()
{
    SAFE_DELETE(m_camera);
    SAFE_DELETE(m_rootScene);
}

void WireframeApp::update(float dt)
{
    m_sphere->applyRotation(4.9f * dt, blink::VEC3_PX);
    m_sphere->applyRotation(6.9f * dt, blink::VEC3_PY);

    m_cube->applyRotation(-4.9f * dt, blink::VEC3_PX);
    m_cube->applyRotation(6.9f * dt, blink::VEC3_PY);

    m_rootScene->updateWorldTransform(blink::MAT4_IDENTITY);
}

void WireframeApp::render()
{
    blink::RenderModule* pRenderModule = blink::Framework::getInstance().findComponent<blink::RenderModule>();
    if (!pRenderModule) return;

    pRenderModule->render(m_rootScene, m_camera);
}

int main(int argc, char** argv)
{
    return blink::Framework::getInstance().startup(new WireframeApp());
}
