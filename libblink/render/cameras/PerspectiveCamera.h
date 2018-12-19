#pragma once
#include "Camera.h"

namespace blink
{
    class PerspectiveCamera : public Camera
    {
    public:
        RTTI_DEF(PerspectiveCamera, Camera);

        PerspectiveCamera();
        virtual ~PerspectiveCamera();

        void setPosition(const glm::vec3& pos);
        void setTargetPosition(const glm::vec3& pos);
        void setUpDirection(const glm::vec3& up);

        const glm::vec3& getPosition() const { return m_eye; }
        const glm::vec3& getTargetPosition() const { return m_target; }
        const glm::vec3& getUpDirection() const { return m_up; }

        void setViewport(float width, float height);
        void setFov(float fov);
        void setClipPanel(float near, float far);

        float getViewportWidth() const { return m_width; }
        float getViewportHeight() const { return m_height; }
        float getFov() const { return m_fov; }
        float getNearClipPlane() const { return m_near; }
        float getFarClipPlane() const { return m_far; }

    protected:
        virtual void updateTransform() override;

    private:
        glm::vec3 m_eye;
        glm::vec3 m_target;
        glm::vec3 m_up{ blink::VEC3_PY };

        float m_width{};
        float m_height{};
        float m_fov{ 45.0f };
        float m_near{ 1.0f };
        float m_far{ 1000.0f };

    };
}