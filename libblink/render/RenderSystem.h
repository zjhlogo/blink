#pragma once
#include <entityx/entityx.h>

namespace blink
{
    class RenderSystem : public entityx::System<RenderSystem>
    {
    public:
        virtual ~RenderSystem() {};

        void setCamera(entityx::Entity& camera) { m_camera = camera; };

    protected:
        entityx::Entity m_camera;

    };
}
