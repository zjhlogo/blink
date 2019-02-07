#pragma once
#include <entityx/entityx.h>
#include <camera/CameraData.h>
#include <input/InputData.h>

class SideViewCameraSystem : public entityx::System<SideViewCameraSystem>, public entityx::Receiver<SideViewCameraSystem>
{
public:
    virtual ~SideViewCameraSystem();

    void configure(entityx::EventManager &events) override;
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

    void receive(const blink::MouseEvent& evt);
    void receive(const entityx::ComponentAddedEvent<blink::CameraData>& evt);

private:
    entityx::Entity m_camera;

    glm::vec2 m_lastMousePos{};

    float m_zoom{ 1.0f };

    float m_moveSensitivity{ 0.01f };
    float m_scrollSensitivity{ 0.5f };
    bool m_enableMove{};

};
