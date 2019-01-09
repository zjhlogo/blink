#pragma once
#include "core/Entity.h"

namespace blink
{
    class IApp;
    class Framework : public Entity
    {
    public:
        static Framework& getInstance();

        bool startup(IApp* app);
        void shutdown();

        void step(float dt);

        const glm::vec2& getWindowSize() const { return m_windowSize; };

    private:
        IApp* m_app{};
        glm::vec2 m_windowSize;

    };
}
