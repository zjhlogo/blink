#pragma once
#include "Camera.h"

namespace blink
{
    class TargetCamera : public Camera
    {
    public:
        RTTI_DEF(TargetCamera, Camera);

        TargetCamera();
        virtual ~TargetCamera();

        void setTargetPosition(const glm::vec3& pos);
        void setUpDirection(const glm::vec3& up);

        const glm::vec3& getTargetPosition() const { return m_target; }
        const glm::vec3& getUpDirection() const { return m_up; }

    private:
        virtual void updateTransform() override;

    private:
        glm::vec3 m_target{ VEC3_ZERO };
        glm::vec3 m_up{ VEC3_PY };

    };
}