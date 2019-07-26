#include "RenderSystem.h"

namespace blink
{
    void RenderSystem::configure(entityx::EventManager & events)
    {
        events.subscribe<entityx::ComponentAddedEvent<CameraData>>(*this);
    }

    void RenderSystem::receive(const entityx::ComponentAddedEvent<CameraData>& evt)
    {
        m_camera = evt.entity;
    }
}
