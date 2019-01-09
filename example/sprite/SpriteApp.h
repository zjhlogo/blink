#pragma once
#include <IApp.h>
#include <objects/Scene.h>
#include "Sprite.h"

class SpriteApp : public blink::IApp
{
public:
    RTTI_DEF(SpriteApp, blink::IApp);

    SpriteApp();
    virtual ~SpriteApp();

    virtual bool initialize() override;
    virtual void terminate() override;

    virtual void update(float dt) override;
    virtual void render() override;

private:
    virtual blink::RenderModule* createRenderer() override;

private:
    blink::Scene* m_rootScene{};
    blink::Camera* m_camera{};

    Sprite* m_sprite{};

};
