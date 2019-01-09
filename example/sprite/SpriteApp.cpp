#include "SpriteApp.h"
#include "SpriteRenderModule.h"
#include <Framework.h>
#include <cameras/FixCamera.h>

SpriteApp::SpriteApp()
    :IApp(1280, 720, "Sprite")
{
}

SpriteApp::~SpriteApp()
{
}

bool SpriteApp::initialize()
{
    m_rootScene = new blink::Scene();

    m_sprite = new Sprite();
    m_sprite->setHead(1);
    m_sprite->setBody(1);
    m_sprite->setLeg(1);
    m_sprite->setArm(1);
    m_rootScene->add(m_sprite);

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
    m_sprite->update(dt);
    m_rootScene->updateWorldTransform(blink::MAT4_IDENTITY);
}

void SpriteApp::render()
{
    renderObject(m_rootScene, m_camera);
}

blink::RenderModule * SpriteApp::createRenderer()
{
    return new SpriteRenderModule();
}

int main(int argc, char** argv)
{
    return blink::Framework::getInstance().startup(new SpriteApp());
}
