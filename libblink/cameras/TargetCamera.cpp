#include "TargetCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace blink
{
    TargetCamera::TargetCamera()
    {

    }

    TargetCamera::~TargetCamera()
    {

    }

    void TargetCamera::setTargetPosition(const glm::vec3 & pos)
    {
        m_target = pos;
        m_transformDirty = true;
    }

    void TargetCamera::setUpDirection(const glm::vec3 & up)
    {
        m_up = up;
        m_transformDirty = true;
    }

    void TargetCamera::updateTransform()
    {
        m_worldToCamera = glm::lookAt(m_position, m_target, m_up);
        m_worldToClip = m_cameraToClip * m_worldToCamera;
    }
}
