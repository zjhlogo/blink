#pragma once
#include "RenderSystem.h"
#include <glm/glm.hpp>

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

        glm::vec3 m_cameraPos;
        glm::mat4 m_worldToCamera;
        glm::mat4 m_cameraToClip;
        glm::mat4 m_worldToClip;

    };
}
