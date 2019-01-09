#include "SpriteApp.h"
#include "SpriteGeometry.h"
#include "SpriteMaterial.h"
#include <Framework.h>
#include <cameras/FixCamera.h>

SpriteApp::SpriteApp()
    :IApp(1280, 720, "SpriteMesh")
{
}

SpriteApp::~SpriteApp()
{
}

bool SpriteApp::initialize()
{
    m_rootScene = new blink::Scene();

    auto geometry = new SpriteGeometry(glm::vec2(40.0f, 56.0f), 20, 1);

    // leg
    {
        auto material = new SpriteMaterial();
        material->setTexture("tex_diffuse", "resource/armor/leg/Armor_Legs_1.png", 0);
        auto mesh = new blink::Mesh(geometry, material);
        m_rootScene->add(mesh);
    }

    // body
    {
        auto material = new SpriteMaterial();
        material->setTexture("tex_diffuse", "resource/armor/body/Armor_Body_1.png", 0);
        auto mesh = new blink::Mesh(geometry, material);
        m_rootScene->add(mesh);
    }

    // head
    {
        auto material = new SpriteMaterial();
        material->setTexture("tex_diffuse", "resource/armor/head/Armor_Head_1.png", 0);
        auto mesh = new blink::Mesh(geometry, material);
        m_rootScene->add(mesh);
    }

    // arm
    {
        auto material = new SpriteMaterial();
        material->setTexture("tex_diffuse", "resource/armor/arm/Armor_Arm_1.png", 0);
        auto mesh = new blink::Mesh(geometry, material);
        m_rootScene->add(mesh);
    }

    auto camera = new blink::FixCamera();
    camera->useDefaultProjection(blink::Camera::DefaultProjection::Orthographic);
    camera->lookAt({ 0.0f, 0.0f, 3.0f }, blink::VEC3_ZERO, blink::VEC3_PY);
    m_camera = camera;

    return true;
}

void SpriteApp::terminate()
{
    SAFE_DELETE(m_camera);
    SAFE_DELETE(m_rootScene);
}

void SpriteApp::update(float dt)
{
    m_rootScene->updateWorldTransform(blink::MAT4_IDENTITY);
}

void SpriteApp::render()
{
    renderObject(m_rootScene, m_camera);
}

blink::RenderModule * SpriteApp::createRenderer()
{
    return nullptr;
}

int main(int argc, char** argv)
{
    return blink::Framework::getInstance().startup(new SpriteApp());
}
