#pragma once
#include "Camera.h"

namespace blink
{
    class Shader;

    class OrthographicCamera : public Camera
    {
    public:
        RTTI_DEF(OrthographicCamera, Camera);

        OrthographicCamera();
        virtual ~OrthographicCamera();

        void setViewport(float left, float right, float bottom, float top);
        void setClipPanel(float near, float far);

        float getViewportLeft() const { return m_left; }
        float getViewportRight() const { return m_right; }
        float getViewportBottom() const { return m_bottom; }
        float getViewportTop() const { return m_top; }
        float getNearClipPlane() const { return m_near; }
        float getFarClipPlane() const { return m_far; }

        virtual void setupShaderUniforms(const glm::mat4& localToWorld, Shader* shader) override;

    protected:
        virtual void updateTransform() override;

    private:
        float m_left{};
        float m_right{};
        float m_bottom{};
        float m_top{};
        float m_near{ 1.0f };
        float m_far{ 1000.0f };

    };
}
