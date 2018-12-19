#pragma once
#include "../core/Component.h"
#include "RenderState.h"
#include "objects/Object.h"
#include "cameras/Camera.h"

namespace blink
{
    class RenderModule : public Component
    {
        friend RenderState;

    public:
        RTTI_DEF(RenderModule, Component);

    public:
        RenderModule();
        virtual ~RenderModule();

        virtual bool initialize(Entity* pEntity) override;
        virtual void terminate() override;

        void reset();
        void applyCurrentRenderState(bool force = true);

        void renderObject(Object* object, Camera* camera);

    private:
        RenderState::StateInfo m_currentRenderState;
        RenderState::StateInfo m_deviceRenderState;
    };
}
