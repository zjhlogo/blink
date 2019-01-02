#pragma once
#include "FixCamera.h"
#include "../input/MouseComponent.h"

namespace blink
{
    class TargetCamera : public FixCamera
    {
        DECLARE_EVENT_HOST();

    public:
        RTTI_DEF(TargetCamera, FixCamera);

        TargetCamera();
        virtual ~TargetCamera();

        virtual void lookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up) override;

    private:
        void onMouseEvent(MouseEvent& evt);
        void clampOffset(glm::vec2& offset);

    private:
        float m_radius{};
        glm::vec2 m_lastMousePos{};
        glm::vec2 m_offset{};
        float m_moveSensitivity{ 0.01f };
        float m_scrollSensitivity{ -1.0f };
        bool m_enableMove{};

    };
}
