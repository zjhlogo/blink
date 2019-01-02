#include "Camera.h"
#include "../Framework.h"
#include "../shaders/Shader.h"
#include <glm/gtc/matrix_transform.hpp>

namespace blink
{
    Camera::Camera()
    {
        useDefaultProjection(DefaultProjection::Perspective);
    }

    Camera::~Camera()
    {
    }

    void Camera::useDefaultProjection(DefaultProjection projection)
    {
        switch (projection)
        {
        case DefaultProjection::Perspective:
        {
            const auto& windowSize = Framework::getInstance().getWindowSize();
            setPerspectiveProjection(45.0f, windowSize.x, windowSize.y, 0.1f, 1000.0f);
        }
        break;
        case DefaultProjection::Orthographic:
        {
            const auto& windowSize = Framework::getInstance().getWindowSize();
            setOrthographicProjection(-0.5f * windowSize.x, 0.5f * windowSize.x, -0.5f * windowSize.y, 0.5f * windowSize.y, 0.1f, 1000.0f);
        }
        break;
        }
    }

    void Camera::setOrthographicProjection(float left, float right, float bottom, float top, float near, float far)
    {
        m_cameraToClip = glm::ortho(left, right, bottom, top, near, far);
        m_transformDirty = true;
    }

    void Camera::setPerspectiveProjection(float fov, float width, float height, float near, float far)
    {
        m_cameraToClip = glm::perspectiveFov(fov, width, height, near, far);
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

    void Camera::setupShaderUniforms(const glm::mat4 & localToWorld, Shader * shader)
    {
        const glm::mat4& worldToClip = getWorldToClipTransform();
        shader->setUniform("u_worldToClip", worldToClip);
        shader->setUniform("u_localToWorld", localToWorld);
        shader->setUniform("u_localToWorldTranInv", glm::transpose(glm::inverse(glm::mat3(localToWorld))));
        shader->setUniform("u_localToClip", worldToClip * localToWorld);
        shader->setUniform("u_viewPos", getPosition());
    }
}
