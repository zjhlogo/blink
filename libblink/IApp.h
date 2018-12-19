#pragma once

namespace blink
{
    class IApp
    {
    public:
        virtual bool initialize() = 0;
        virtual void terminate() = 0;

        virtual void update(float dt);
        virtual void render();

    };
}
