#include "NormalMappingApp.h"
#include "NormalMappingMaterial.h"
#include <Framework.h>
#include <geometries/PlaneGeometry.h>
#include <geometries/BoxGeometry.h>
#include <lights/PointLight.h>
#include <render/RenderModule.h>
#include <cameras/TargetCamera.h>
#include <Log.h>

NormalMappingApp::NormalMappingApp()
    :IApp(1280, 720, "Normal Mapping")
{

}

NormalMappingApp::~NormalMappingApp()
{

}

bool NormalMappingApp::initialize()
{
    m_rootScene = new blink::Scene();

    auto material = new NormalMappingMaterial();
    material->setTexture("tex_diffuse", "resource/brickwall.jpg", 0);
    material->setTexture("tex_normal", "resource/brickwall_normal.jpg", 1);

    auto planeGeo = new blink::PlaneGeometry(
        1.0f, 1.0f,
        blink::PlaneGeometry::Facing::PositiveZ,
        1, 1,
        blink::BufferAttributes::StockAttributes::Pos3Uv2NormalTangent);

    m_plane = new blink::Mesh(planeGeo, material);
    m_plane->setPosition({ -1.0f, 0.0f, 0.0f });
    m_rootScene->add(m_plane);

    auto cubeGeo = new blink::BoxGeometry(1.0f, 1.0f, 1.0f, 1, 1, 1, blink::BufferAttributes::StockAttributes::Pos3Uv2NormalTangent);
    auto cubeMesh = new blink::Mesh(cubeGeo, material);
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

void NormalMappingApp::terminate()
{
    SAFE_DELETE(m_camera);
    SAFE_DELETE(m_rootScene);
}

void NormalMappingApp::update(float dt)
{
    m_rootScene->updateWorldTransform(blink::MAT4_IDENTITY);
}

void NormalMappingApp::render()
{
    renderObject(m_rootScene, m_camera);
}

int main(int argc, char** argv)
{
    return blink::Framework::getInstance().startup(new NormalMappingApp());
}
