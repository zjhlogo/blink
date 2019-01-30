#pragma once
#include "RenderSystem.h"
#include "RenderData.h"

namespace blink
{
    class Shader;

    class OpenGL3RenderSystem: public RenderSystem
    {
    public:
        virtual ~OpenGL3RenderSystem();

        void configure(entityx::EventManager &events) override;
        void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

    private:
        Shader* m_shader{};

    };
}
