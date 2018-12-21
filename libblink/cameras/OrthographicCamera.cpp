#include "OrthographicCamera.h"
#include "../Framework.h"
#include "../shaders/Shader.h"
#include <glm/gtc/matrix_transform.hpp>

namespace blink
{
    OrthographicCamera::OrthographicCamera()
    {
        const auto& windowSize = Framework::getInstance().getWindowSize();
        m_left = -0.5f * windowSize.x;
        m_right = 0.5f * windowSize.x;
        m_bottom = -0.5f * windowSize.y;
        m_top = 0.5f * windowSize.y;
    }

    OrthographicCamera::~OrthographicCamera()
    {

    }

    void OrthographicCamera::setViewport(float left, float right, float bottom, float top)
    {
        m_left = left;
        m_right = right;
        m_bottom = bottom;
        m_top = top;
        m_transformDirty = true;
    }

    void OrthographicCamera::setClipPanel(float near, float far)
    {
        m_near = near;
        m_far = far;
        m_transformDirty = true;
    }

    void OrthographicCamera::setupShaderUniforms(const glm::mat4 & localToWorld, Shader * shader)
    {
        const glm::mat4& worldToClip = getWorldToClipTransform();
        shader->setUniform("u_worldToClip", worldToClip);
        shader->setUniform("u_localToWorld", localToWorld);
        shader->setUniform("u_localToWorldTranInv", glm::transpose(glm::inverse(glm::mat3(localToWorld))));
        shader->setUniform("u_localToClip", worldToClip * localToWorld);
        shader->setUniform("u_viewPos", getPosition());
    }

    void OrthographicCamera::updateTransform()
    {
        m_worldToCamera = glm::lookAt(m_eye, m_target, m_up);
        m_cameraToClip = glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far);
        m_worldToClip = m_cameraToClip * m_worldToCamera;
    }
}
