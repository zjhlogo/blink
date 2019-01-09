#include "MandelbrotApp.h"
#include "MandelbrotMaterial.h"
#include <geometries/PlaneGeometry.h>
#include <Framework.h>
#include <cameras/FixCamera.h>

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

    auto camera = new blink::FixCamera();
    camera->useDefaultProjection(blink::Camera::DefaultProjection::Orthographic);
    camera->lookAt({ 0.0f, 0.0f, 3.0f }, blink::VEC3_ZERO, blink::VEC3_PY);
    m_camera = camera;

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
    renderObject(m_rootScene, m_camera);
}

int main(int argc, char** argv)
{
    return blink::Framework::getInstance().startup(new MandelbrotApp());
}
