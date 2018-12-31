#include "MandelbrotApp.h"
#include "MandelbrotMaterial.h"
#include <geometries/PlaneGeometry.h>
#include <Framework.h>
#include <render/RenderModule.h>

MandelbrotApp::MandelbrotApp()
    :IApp(1280, 720, "Mandelbrot")
{
}

MandelbrotApp::~MandelbrotApp()
{
}

bool MandelbrotApp::initialize()
{
    m_rootScene = new blink::Scene();

    blink::Mesh* plane = new blink::Mesh(new blink::PlaneGeometry(1280, 720, blink::PlaneGeometry::Facing::PositiveZ), new MandelbrotMaterial());
    m_rootScene->add(plane);

    m_camera = new blink::TargetCamera();
    m_camera->useDefaultProjection(blink::Camera::DefaultProjection::Orthographic);
    m_camera->setPosition({ 0.0f, 0.0f, 3.0f });
    m_camera->setTargetPosition(blink::VEC3_ZERO);

    return true;
}

void MandelbrotApp::terminate()
{
    SAFE_DELETE(m_camera);
    SAFE_DELETE(m_rootScene);
}

void MandelbrotApp::update(float dt)
{
    m_rootScene->updateWorldTransform(blink::MAT4_IDENTITY);
}

void MandelbrotApp::render()
{
    blink::RenderModule* pRenderModule = blink::Framework::getInstance().findComponent<blink::RenderModule>();
    if (!pRenderModule) return;

    pRenderModule->render(m_rootScene, m_camera);
}

int main(int argc, char** argv)
{
    return blink::Framework::getInstance().startup(new MandelbrotApp());
}
