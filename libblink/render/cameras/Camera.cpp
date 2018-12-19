#include "Camera.h"

namespace blink
{
    Camera::Camera()
    {
    }

    Camera::~Camera()
    {
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
