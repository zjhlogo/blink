#pragma once
#include <entityx/entityx.h>
#include "../camera/CameraData.h"

namespace blink
{
    class RenderSystem : public entityx::System<RenderSystem>, public entityx::Receiver<RenderSystem>
    {
    public:
        virtual ~RenderSystem() {};

        void configure(entityx::EventManager &events) override;

        void receive(const entityx::ComponentAddedEvent<CameraData>& evt);

    protected:
        entityx::Entity m_camera;

    };
}
