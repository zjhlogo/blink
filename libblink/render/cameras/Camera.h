#pragma once
#include <Rtti.h>
#include <glm/glm.hpp>

namespace blink
{
    class Shader;

    class Camera
    {
    public:
        RTTI_ROOT(Camera);

        Camera();
        virtual ~Camera();

        void setPosition(const glm::vec3& pos);
        void setTargetPosition(const glm::vec3& pos);
        void setUpDirection(const glm::vec3& up);

        const glm::vec3& getPosition() const { return m_eye; }
        const glm::vec3& getTargetPosition() const { return m_target; }
        const glm::vec3& getUpDirection() const { return m_up; }

        const glm::mat4& getWorldToCameraTransform();
        const glm::mat4& getCameraToClipTransform();
        const glm::mat4& getWorldToClipTransform();

        virtual void setupShaderUniforms(const glm::mat4& localToWorld, Shader* shader) = 0;

    protected:
        virtual void updateTransform() = 0;

    protected:
        glm::vec3 m_eye;
        glm::vec3 m_target;
        glm::vec3 m_up{ blink::VEC3_PY };

        glm::mat4 m_worldToCamera;
        glm::mat4 m_cameraToClip;
        glm::mat4 m_worldToClip;
        bool m_transformDirty{ true };

    };
}
