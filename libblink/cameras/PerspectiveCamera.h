#pragma once
#include "Camera.h"

namespace blink
{
    class Shader;

    class PerspectiveCamera : public Camera
    {
    public:
        RTTI_DEF(PerspectiveCamera, Camera);

        PerspectiveCamera();
        virtual ~PerspectiveCamera();

        void setViewport(float width, float height);
        void setFov(float fov);
        void setClipPanel(float near, float far);

        float getViewportWidth() const { return m_width; }
        float getViewportHeight() const { return m_height; }
        float getFov() const { return m_fov; }
        float getNearClipPlane() const { return m_near; }
        float getFarClipPlane() const { return m_far; }

        virtual void setupShaderUniforms(const glm::mat4& localToWorld, Shader* shader) override;

    protected:
        virtual void updateTransform() override;

    private:
        float m_width{};
        float m_height{};
        float m_fov{ 45.0f };
        float m_near{ 1.0f };
        float m_far{ 1000.0f };

    };
}