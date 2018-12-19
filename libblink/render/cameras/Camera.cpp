#include "Camera.h"

namespace blink
{
    Camera::Camera()
    {
    }

    Camera::~Camera()
    {
    }

    void Camera::setPosition(const glm::vec3 & pos)
    {
        m_eye = pos;
        m_transformDirty = true;
    }

    void Camera::setTargetPosition(const glm::vec3 & pos)
    {
        m_target = pos;
        m_transformDirty = true;
    }

    void Camera::setUpDirection(const glm::vec3 & up)
    {
        m_up = up;
        m_transformDirty = true;
    }

    const glm::mat4 & Camera::getWorldToCameraTransform()
    {
        if (m_transformDirty)
        {
            updateTransform();
            m_transformDirty = false;
        }

        return m_worldToCamera;
    }

    const glm::mat4 & Camera::getCameraToClipTransform()
    {
        if (m_transformDirty)
        {
            updateTransform();
            m_transformDirty = false;
        }

        return m_cameraToClip;
    }

    const glm::mat4 & Camera::getWorldToClipTransform()
    {
        if (m_transformDirty)
        {
            updateTransform();
            m_transformDirty = false;
        }

        return m_worldToClip;
    }
}
