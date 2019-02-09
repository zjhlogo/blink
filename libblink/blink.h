#pragma once
#include <entityx/entityx.h>

namespace blink
{
    class App
    {
    public:
        virtual bool initialize() = 0;
        virtual void terminate() = 0;

        virtual void step(float dt);

    public:
        entityx::EntityX m_ex;

    };

    int run(App* app);
}
