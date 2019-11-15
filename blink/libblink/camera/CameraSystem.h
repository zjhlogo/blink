/*!
 * \file CameraSystem.h
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 * 
 */
#pragma once
#include <entityx/entityx.h>
#include "CameraData.h"
#include "../input/InputData.h"

NS_BEGIN

class CameraSystem : public entityx::System<CameraSystem>, public entityx::Receiver<CameraSystem>
{
public:
    virtual ~CameraSystem();

    void configure(entityx::EventManager &events) override;
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

    void receive(const MouseEvent& evt);
    void receive(const entityx::ComponentAddedEvent<CameraData>& evt);

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

NS_END
