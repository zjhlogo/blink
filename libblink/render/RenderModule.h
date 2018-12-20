#pragma once
#include "../core/Component.h"
#include "../objects/Mesh.h"
#include "../lights/Light.h"
#include "../cameras/Camera.h"
#include "RenderState.h"

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

        void render(Object* rootObj, Camera* camera);

    private:
        void renderObject(Mesh* mesh, Camera* camera, const std::vector<Light*>& lights);

    private:
        RenderState::StateInfo m_currentRenderState;
        RenderState::StateInfo m_deviceRenderState;
    };
}
