#include "HelloWorldApp.h"
#include <Framework.h>
#include <geometries/BoxGeometry.h>
#include <geometries/PlaneGeometry.h>
#include <textures/Texture2D.h>
#include <materials/LambertMaterial.h>
#include <materials/WireframeMaterial.h>
#include <lights/AmbientLight.h>
#include <lights/PointLight.h>
#include <render/RenderModule.h>

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

    m_cube = new blink::Mesh(new blink::BoxGeometry(1.0f, 1.0f, 1.0f), new blink::LambertMaterial());
    m_rootScene->add(m_cube);

    blink::Material* material = new blink::LambertMaterial();
    material->setTexture(blink::Material::TextureType::Diffuse, blink::Texture2D::fromFile("grid16.png"));
    blink::Mesh* plane = new blink::Mesh(new blink::PlaneGeometry(10.0f, 10.0f, blink::PlaneGeometry::Facing::PositiveY), material);
    plane->setPosition({ 0.0f, -3.0f, 0.0f });
    m_rootScene->add(plane);

    m_rootScene->add(new blink::AmbientLight());
    blink::PointLight* light = new blink::PointLight();
    light->setPosition({ 0.0f, 5.0f, 1.0f });
    m_rootScene->add(light);

    m_camera = new blink::PerspectiveCamera();
    m_camera->setPosition({ 0.0f, 3.0f, 3.0f });
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
    m_cube->applyRotation(29.0f * dt, blink::VEC3_PX);
    m_cube->applyRotation(39.0f * dt, blink::VEC3_PY);
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
