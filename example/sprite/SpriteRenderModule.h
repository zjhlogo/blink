#pragma once
#include <render/RenderModule.h>
#include "SpriteGeometry.h"
#include "SpriteMaterial.h"

class SpriteRenderModule : public blink::RenderModule
{
public:
    RTTI_DEF(SpriteRenderModule, blink::RenderModule);

public:
    SpriteRenderModule();
    virtual ~SpriteRenderModule();

    virtual bool initialize(blink::Entity* pEntity) override;
    virtual void terminate() override;

    virtual void render(blink::Object* rootObj, blink::Camera* camera) override;

private:
    SpriteGeometry m_spriteBatchGeometry;
    SpriteMaterial* m_spriteMaterial{};

};
