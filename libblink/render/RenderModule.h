#pragma once
#include "../core/Component.h"
#include "RenderState.h"

namespace blink
{
    class RenderState;

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

        const glm::ivec2& getSurfaceSize() const { return m_surfaceSize; };

        void reset();
        void applyCurrentRenderState(bool force = true);

    private:
        glm::ivec2 m_surfaceSize;

        RenderState::StateInfo m_currentRenderState;
        RenderState::StateInfo m_deviceRenderState;
    };
}
