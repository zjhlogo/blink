#pragma once
#include "core/Entity.h"

namespace blink
{
    class IApp;
    class Framework : public Entity
    {
    public:
        static Framework& getInstance();

        bool initialize(IApp* app);
        int start();

        void step(float dt);
        void resize();
        void reload();

    private:
        IApp* m_app{};

    };
}
