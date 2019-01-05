#include "HelloWorldApp.h"
#include <Framework.h>
#include <geometries/BoxGeometry.h>
#include <geometries/PlaneGeometry.h>
#include <geometries/SphereGeometry.h>
#include <textures/Texture2D.h>
#include <materials/LambertMaterial.h>
#include <lights/AmbientLight.h>
#include <lights/PointLight.h>
#include <render/RenderModule.h>
#include <cameras/TargetCamera.h>
#include <Log.h>

HelloWorldApp::HelloWorldApp()
    :IApp(1280, 720, "Hello World")
{

}

HelloWorldApp::~HelloWorldApp()
{

}

bool HelloWorldApp::initialize()
{
    m_rootScene = new blink::Scene();

    auto colorMaterial = new blink::LambertMaterial();
    m_cube = new blink::Mesh(new blink::BoxGeometry(1.0f, 1.0f, 1.0f), colorMaterial);
    m_rootScene->add(m_cube);

    blink::Material* textureMaterial = new blink::LambertMaterial();
    textureMaterial->setTexture("tex_diffuse", "resource/grid16.png", 0);
    blink::Mesh* plane = new blink::Mesh(new blink::PlaneGeometry(10.0f, 10.0f, blink::PlaneGeometry::Facing::PositiveY), textureMaterial);
    plane->setPosition({ 0.0f, -3.0f, 0.0f });
    m_rootScene->add(plane);

    m_rootScene->add(new blink::AmbientLight());
    blink::PointLight* light = new blink::PointLight();
    light->setPosition({ 2.0f, 2.0f, 2.0f });
    m_rootScene->add(light);

    auto lightGeo = new blink::Mesh(new blink::SphereGeometry(0.1f, 5, 10), colorMaterial);
    lightGeo->setPosition(light->getPosition());
    m_rootScene->add(lightGeo);

    auto camera = new blink::TargetCamera();
    camera->lookAt({ -3.0f, 3.0f, 3.0f }, blink::VEC3_ZERO, blink::VEC3_PY);
    m_camera = camera;

    return true;
}

void HelloWorldApp::terminate()
{
    SAFE_DELETE(m_camera);
    SAFE_DELETE(m_rootScene);
}

void HelloWorldApp::update(float dt)
{
    m_cube->applyRotation(4.9f * dt, blink::VEC3_PX);
    m_cube->applyRotation(6.9f * dt, blink::VEC3_PY);
    m_rootScene->updateWorldTransform(blink::MAT4_IDENTITY);
}

void HelloWorldApp::render()
{
    blink::RenderModule* pRenderModule = blink::Framework::getInstance().findComponent<blink::RenderModule>();
    if (!pRenderModule) return;

    pRenderModule->render(m_rootScene, m_camera);
}

int main(int argc, char** argv)
{
    return blink::Framework::getInstance().startup(new HelloWorldApp());
}
