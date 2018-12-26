#include "WireframeApp.h"
#include <Framework.h>
#include <geometries/SphereGeometry.h>
#include <geometries/PlaneGeometry.h>
#include <materials/LambertMaterial.h>
#include <materials/WireframeMaterial.h>
#include <lights/AmbientLight.h>
#include <lights/PointLight.h>
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

    m_sphere = new blink::Mesh(new blink::SphereGeometry(1.0f, 20, 20), new blink::WireframeMaterial());
    m_rootScene->add(m_sphere);

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
    m_sphere->applyRotation(29.0f * dt, blink::VEC3_PX);
    m_sphere->applyRotation(59.0f * dt, blink::VEC3_PY);
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
