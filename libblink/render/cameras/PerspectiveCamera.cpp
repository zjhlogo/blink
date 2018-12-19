#include "PerspectiveCamera.h"
#include "../../Framework.h"
#include <glm/gtc/matrix_transform.hpp>

namespace blink
{
    PerspectiveCamera::PerspectiveCamera()
    {
        const auto& windowSize = Framework::getInstance().getWindowSize();
        m_width = windowSize.x;
        m_height = windowSize.y;
    }

    PerspectiveCamera::~PerspectiveCamera()
    {

    }

    void PerspectiveCamera::setViewport(float width, float height)
    {
        m_width = width;
        m_height = height;
        m_transformDirty = true;
    }

    void PerspectiveCamera::setFov(float fov)
    {
        m_fov = fov;
        m_transformDirty = true;
    }

    void PerspectiveCamera::setClipPanel(float near, float far)
    {
        m_near = near;
        m_far = far;
        m_transformDirty = true;
    }

    void PerspectiveCamera::updateTransform()
    {
        m_worldToCamera = glm::lookAt(m_eye, m_target, m_up);
        m_cameraToClip = glm::perspectiveFov(m_fov, m_width, m_height, m_near, m_far);
        m_worldToClip = m_cameraToClip * m_worldToCamera;
    }
}
