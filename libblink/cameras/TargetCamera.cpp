#include "TargetCamera.h"
#include "../Framework.h"
#include <Log.h>
#include <glm/gtc/matrix_transform.hpp>

namespace blink
{
    TargetCamera::TargetCamera()
    {
        MouseComponent* mouseComponent = Framework::getInstance().findComponent<MouseComponent>();
        REGISTER_EVENT(mouseComponent, MouseEvent, &TargetCamera::onMouseEvent);
    }

    TargetCamera::~TargetCamera()
    {

    }

    void TargetCamera::lookAt(const glm::vec3 & pos, const glm::vec3 & target, const glm::vec3 & up)
    {
        m_position = pos;
        m_target = target;
        m_up = up;

        glm::vec3 distance = pos - target;
        m_radius = glm::length(distance);

        // calculate offset from position
        m_offset.t = acos(distance.y / m_radius);
        clampOffset(m_offset);
        m_offset.s = asin(distance.x / (m_radius * sin(m_offset.t)));

        m_transformDirty = true;
    }

    void TargetCamera::updateTransform()
    {
        m_worldToCamera = glm::lookAt(m_position, m_target, m_up);
        m_worldToClip = m_cameraToClip * m_worldToCamera;
    }

    void TargetCamera::onMouseEvent(MouseEvent & evt)
    {
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
                m_position = m_target + distance;

                m_transformDirty = true;
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
            glm::vec3 distance = m_position - m_target;
            m_radius += m_scrollSensitivity * evt.mouseScroll.y;
            if (m_radius < 0.1f) m_radius = 0.1f;
            m_position = m_target + glm::normalize(distance) * m_radius;
            m_transformDirty = true;
        }
        break;
        default:
        break;
        }
    }

    void TargetCamera::clampOffset(glm::vec2 & offset)
    {
        static const float MIN_RADIANS = glm::radians(1.0f);
        static const float MAX_RADIANS = glm::radians(179.0f);

        if (offset.t < MIN_RADIANS) offset.t = MIN_RADIANS;
        if (offset.t > MAX_RADIANS) offset.t = MAX_RADIANS;
    }
}
