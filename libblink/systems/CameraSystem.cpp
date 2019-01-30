#include "CameraSystem.h"
#include "../Components.h"

namespace blink
{
    CameraSystem::~CameraSystem()
    {

    }

    void CameraSystem::configure(entityx::EventManager & events)
    {
        events.subscribe<MouseEvent>(*this);
    }

    void CameraSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
    {
        if (!m_camera.valid()) return;

        auto cameraData = m_camera.component<CameraData>().get();
        if (cameraData->bitFlag & TransformFlag::TRANSFORM_LOCAL_DIRTY)
        {
            cameraData->cameraToClip = glm::perspectiveFov(45.0f, 1280.0f, 720.0f, 0.1f, 100.0f);
            cameraData->worldToCamera = glm::lookAt(cameraData->cameraPos, cameraData->cameraTarget, cameraData->cameraUp);
            cameraData->worldToClip = cameraData->cameraToClip * cameraData->worldToCamera;
            cameraData->bitFlag &= (~TransformFlag::TRANSFORM_LOCAL_DIRTY);
        }
    }

    void CameraSystem::setCamera(entityx::Entity & camera)
    {
        m_camera = camera;
        if (!m_camera.valid()) return;

        auto cameraData = m_camera.component<CameraData>();

        glm::vec3 distance = cameraData->cameraPos - cameraData->cameraTarget;
        m_radius = glm::length(distance);

        // calculate offset from position
        m_offset.t = acos(distance.y / m_radius);
        clampOffset(m_offset);
        m_offset.s = asin(distance.x / (m_radius * sin(m_offset.t)));
    }

    void CameraSystem::receive(const MouseEvent & evt)
    {
        if (!m_camera.valid()) return;

        auto cameraData = m_camera.component<CameraData>();

        switch (evt.action)
        {
        case MouseEvent::Action::Move:
        {
            if (m_enableMove)
            {
                glm::vec2 detail = { m_lastMousePos.x - evt.mousePos.x, m_lastMousePos.y - evt.mousePos.y };
                detail *= m_moveSensitivity;
                m_offset += detail;
                clampOffset(m_offset);

                // calculate new position
                glm::vec3 distance;
                distance.x = m_radius * sin(m_offset.s) * sin(m_offset.t);
                distance.y = m_radius * cos(m_offset.t);
                distance.z = m_radius * cos(m_offset.s) * sin(m_offset.t);
                cameraData->cameraPos = cameraData->cameraTarget + distance;
                cameraData->bitFlag |= TransformFlag::TRANSFORM_LOCAL_DIRTY;
            }

            m_lastMousePos = evt.mousePos;
        }
        break;
        case MouseEvent::Action::ButtonDown:
        {
            m_enableMove = true;
        }
        break;
        case MouseEvent::Action::ButtonUp:
        {
            m_enableMove = false;
        }
        break;
        case MouseEvent::Action::Scroll:
        {
            glm::vec3 distance = cameraData->cameraPos - cameraData->cameraTarget;
            m_radius += m_scrollSensitivity * evt.mousePos.y;
            if (m_radius < 0.1f) m_radius = 0.1f;
            cameraData->cameraPos = cameraData->cameraTarget + glm::normalize(distance) * m_radius;
            cameraData->bitFlag |= TransformFlag::TRANSFORM_LOCAL_DIRTY;
        }
        break;
        default:
            break;
        }
    }

    void CameraSystem::clampOffset(glm::vec2 & offset)
    {
        static const float MIN_RADIANS = glm::radians(1.0f);
        static const float MAX_RADIANS = glm::radians(179.0f);

        if (offset.t < MIN_RADIANS) offset.t = MIN_RADIANS;
        if (offset.t > MAX_RADIANS) offset.t = MAX_RADIANS;
    }
}
