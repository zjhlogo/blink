#include "SideViewCameraSystem.h"
#include <glm/gtc/matrix_transform.hpp>

SideViewCameraSystem::~SideViewCameraSystem()
{
}

void SideViewCameraSystem::configure(entityx::EventManager & events)
{
    events.subscribe<NS::MouseEvent>(*this);
    events.subscribe<entityx::ComponentAddedEvent<NS::CameraData>>(*this);
}

void SideViewCameraSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    if (!m_camera.valid()) return;

    auto cameraData = m_camera.component<NS::CameraData>().get();
    if (cameraData->bitFlag & NS::CameraData::BF_DIRTY)
    {
        cameraData->cameraToClip = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f, 1.0f, 100.0f);

        glm::mat4 matScale = glm::scale(NS::MAT4_IDENTITY, glm::vec3(m_zoom, m_zoom, m_zoom));
        cameraData->worldToCamera = glm::lookAt(cameraData->cameraPos, cameraData->cameraTarget, cameraData->cameraUp) * matScale;

        cameraData->worldToClip = cameraData->cameraToClip * cameraData->worldToCamera;
        cameraData->bitFlag &= (~NS::CameraData::BF_DIRTY);
    }
}

void SideViewCameraSystem::receive(const NS::MouseEvent & evt)
{
    if (!m_camera.valid()) return;

    auto cameraData = m_camera.component<NS::CameraData>().get();

    switch (evt.action)
    {
    case NS::MouseEvent::Action::Move:
    {
        if (m_enableMove)
        {
            glm::vec2 detail = { m_lastMousePos.x - evt.mousePos.x, evt.mousePos.y - m_lastMousePos.y };
            detail *= m_moveSensitivity;

            // calculate new position
            cameraData->cameraPos += glm::vec3(detail, 0.0f);
            cameraData->cameraTarget += glm::vec3(detail, 0.0f);
            cameraData->bitFlag |= NS::CameraData::BF_DIRTY;
        }

        m_lastMousePos = evt.mousePos;
    }
    break;
    case NS::MouseEvent::Action::ButtonDown:
    {
        m_enableMove = true;
    }
    break;
    case NS::MouseEvent::Action::ButtonUp:
    {
        m_enableMove = false;
    }
    break;
    case NS::MouseEvent::Action::Scroll:
    {
        if (evt.mousePos.y < 0.0f)
        {
            m_zoom *= (m_zoomSensitivity * -evt.mousePos.y);
        }
        else
        {
            m_zoom /= (m_zoomSensitivity * evt.mousePos.y);
        }

        cameraData->bitFlag |= NS::CameraData::BF_DIRTY;
    }
    break;
    default:
        break;
    }
}

void SideViewCameraSystem::receive(const entityx::ComponentAddedEvent<NS::CameraData>& evt)
{
    m_camera = evt.entity;
}
