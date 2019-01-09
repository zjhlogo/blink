#include "HatchingApp.h"
#include "HatchingMaterial.h"
#include <Framework.h>
#include <geometries/SphereGeometry.h>
#include <geometries/BoxGeometry.h>
#include <lights/AmbientLight.h>
#include <lights/PointLight.h>
#include <cameras/TargetCamera.h>

HatchingApp::HatchingApp()
    :IApp(1280, 720, "Hatching")
{

}

HatchingApp::~HatchingApp()
{

}

bool HatchingApp::initialize()
{
    m_rootScene = new blink::Scene();

    HatchingMaterial* hatchingMaterial = new HatchingMaterial();
    hatchingMaterial->setTexture("tex_diffuse", "resource/hatching.png", 0);
    hatchingMaterial->setTexture("tex_hatchingLevel", "resource/hatchinglevel.png", 1);

    // add sphere
    m_sphere = new blink::Mesh(new blink::SphereGeometry(0.5f, 10, 20), hatchingMaterial);
    m_sphere->setPosition({ -1.0f, 0.0f, 0.0f });
    m_rootScene->add(m_sphere);

    // add cube
    m_cube = new blink::Mesh(new blink::BoxGeometry(1.0f, 1.0f, 1.0f), hatchingMaterial);
    m_cube->setPosition({ 1.0f, 0.0f, 0.0f });
    m_rootScene->add(m_cube);

    m_rootScene->add(new blink::AmbientLight());

    blink::PointLight* light = new blink::PointLight();
    light->setPosition({ 0.0f, 5.0f, 1.0f });
    m_rootScene->add(light);

    auto camera = new blink::TargetCamera();
    camera->lookAt({ 0.0f, 3.0f, 3.0f }, blink::VEC3_ZERO, blink::VEC3_PY);
    m_camera = camera;

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

    m_cube->applyRotation(-4.9f * dt, blink::VEC3_PX);
    m_cube->applyRotation(-6.9f * dt, blink::VEC3_PY);

    m_rootScene->updateWorldTransform(blink::MAT4_IDENTITY);
}

void HatchingApp::render()
{
    renderObject(m_rootScene, m_camera);
}

int main(int argc, char** argv)
{
    return blink::Framework::getInstance().startup(new HatchingApp());
}
