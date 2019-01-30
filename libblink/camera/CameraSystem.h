#pragma once
#include <entityx/entityx.h>
#include "CameraData.h"
#include "../input/InputData.h"

namespace blink
{
    class CameraSystem : public entityx::System<CameraSystem>, public entityx::Receiver<CameraSystem>
    {
    public:
        virtual ~CameraSystem();

        void configure(entityx::EventManager &events) override;
        void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

        void setCamera(entityx::Entity& camera);
        void receive(const MouseEvent& evt);

    private:
        void clampOffset(glm::vec2 & offset);

    private:
        entityx::Entity m_camera;

        float m_radius{};
        glm::vec2 m_lastMousePos{};
        glm::vec2 m_offset{};
        float m_moveSensitivity{ 0.01f };
        float m_scrollSensitivity{ -1.0f };
        bool m_enableMove{};

    };
}
