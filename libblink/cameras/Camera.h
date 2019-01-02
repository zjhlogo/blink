#pragma once
#include <Rtti.h>
#include <glm/glm.hpp>

namespace blink
{
    class Shader;

    class Camera
    {
    public:
        enum class DefaultProjection
        {
            Perspective,
            Orthographic,
        };

    public:
        RTTI_ROOT(Camera);

        Camera();
        virtual ~Camera();

        void useDefaultProjection(DefaultProjection projection);
        void setOrthographicProjection(float left, float right, float bottom, float top, float near, float far);
        void setPerspectiveProjection(float fov, float width, float height, float near, float far);

        virtual const glm::vec3& getPosition() const = 0;

        const glm::mat4& getWorldToCameraTransform();
        const glm::mat4& getCameraToClipTransform();
        const glm::mat4& getWorldToClipTransform();

        virtual void setupShaderUniforms(const glm::mat4& localToWorld, Shader* shader);

    protected:
        virtual void updateTransform() = 0;

    protected:
        glm::mat4 m_worldToCamera;
        glm::mat4 m_cameraToClip;
        glm::mat4 m_worldToClip;
        bool m_transformDirty{ true };

    };
}
