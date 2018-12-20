#include "PerspectiveCamera.h"
#include "../../Framework.h"
#include "../shaders/Shader.h"
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

    void PerspectiveCamera::setupShaderUniforms(const glm::mat4 & localToWorld, Shader* shader)
    {
        const glm::mat4& worldToClip = getWorldToClipTransform();
        shader->setUniform("u_worldToClip", worldToClip);
        shader->setUniform("u_localToWorld", localToWorld);
        shader->setUniform("u_localToWorldTranInv", glm::transpose(glm::inverse(glm::mat3(localToWorld))));
        shader->setUniform("u_localToClip", worldToClip * localToWorld);
        shader->setUniform("u_viewPos", getPosition());
    }

    void PerspectiveCamera::updateTransform()
    {
        m_worldToCamera = glm::lookAt(m_eye, m_target, m_up);
        m_cameraToClip = glm::perspectiveFov(m_fov, m_width, m_height, m_near, m_far);
        m_worldToClip = m_cameraToClip * m_worldToCamera;
    }
}
