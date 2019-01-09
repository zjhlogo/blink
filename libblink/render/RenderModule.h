#pragma once
#include "../core/Component.h"
#include "../objects/Object.h"

namespace blink
{
    class Camera;

    class RenderModule : public Component
    {
    public:
        RTTI_DEF(RenderModule, Component);

    public:
        RenderModule();
        virtual ~RenderModule();

        virtual void render(Object* rootObj, Camera* camera) = 0;

    };
}
