#pragma once
#include <entityx/entityx.h>

namespace blink
{
    class RenderSystem : public entityx::System<RenderSystem>
    {
    public:
        virtual ~RenderSystem() {};

    };
}
