#pragma once
#include "Camera.h"
#include "../input/MouseComponent.h"

namespace blink
{
    class TargetCamera : public Camera
    {
        DECLARE_EVENT_HOST();

    public:
        RTTI_DEF(TargetCamera, Camera);

        TargetCamera();
        virtual ~TargetCamera();

        void lookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);

        virtual const glm::vec3& getPosition() const override { return m_position; }
        const glm::vec3& getTargetPosition() const { return m_target; }
        const glm::vec3& getUpDirection() const { return m_up; }

    private:
        virtual void updateTransform() override;
        void onMouseEvent(MouseEvent& evt);
        void clampOffset(glm::vec2& offset);

    private:
        glm::vec3 m_position{ VEC3_ZERO };
        glm::vec3 m_target{ VEC3_ZERO };
        glm::vec3 m_up{ VEC3_PY };

        float m_radius{};
        glm::vec2 m_lastMousePos{};
        glm::vec2 m_offset{};
        float m_moveSensitivity{ 0.01f };
        float m_scrollSensitivity{ -1.0f };
        bool m_enableMove{};

    };
}
