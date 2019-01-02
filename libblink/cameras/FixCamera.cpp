#include "FixCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace blink
{
    FixCamera::FixCamera()
    {

    }

    FixCamera::~FixCamera()
    {

    }

    void FixCamera::lookAt(const glm::vec3 & pos, const glm::vec3 & target, const glm::vec3 & up)
    {
        m_position = pos;
        m_target = target;
        m_up = up;
        m_transformDirty = true;
    }

    void FixCamera::updateTransform()
    {
        m_worldToCamera = glm::lookAt(m_position, m_target, m_up);
        m_worldToClip = m_cameraToClip * m_worldToCamera;
    }
}
