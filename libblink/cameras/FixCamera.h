#pragma once
#include "Camera.h"

namespace blink
{
    class FixCamera : public Camera
    {
    public:
        RTTI_DEF(FixCamera, Camera);

        FixCamera();
        virtual ~FixCamera();

        virtual void lookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);

        virtual const glm::vec3& getPosition() const override { return m_position; }
        const glm::vec3& getTargetPosition() const { return m_target; }
        const glm::vec3& getUpDirection() const { return m_up; }

    protected:
        virtual void updateTransform() override;

    protected:
        glm::vec3 m_position{ VEC3_ZERO };
        glm::vec3 m_target{ VEC3_ZERO };
        glm::vec3 m_up{ VEC3_PY };

    };
}
