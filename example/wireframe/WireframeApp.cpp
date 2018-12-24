#include "WireframeApp.h"
#include <Framework.h>
#include <geometries/BoxGeometry.h>
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

    m_cube = new blink::Mesh(new blink::BoxGeometry(1.0f, 1.0f, 1.0f, 3, 3, 3), new blink::WireframeMaterial());
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
    m_cube->applyRotation(29.0f * dt, blink::VEC3_PX);
    m_cube->applyRotation(39.0f * dt, blink::VEC3_PY);
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
