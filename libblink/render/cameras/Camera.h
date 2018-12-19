#pragma once
#include <Rtti.h>
#include <glm/glm.hpp>

namespace blink
{
    class Camera
    {
    public:
        RTTI_ROOT(Camera);

        Camera();
        virtual ~Camera();

        const glm::mat4& getWorldToCameraTransform();
        const glm::mat4& getCameraToClipTransform();
        const glm::mat4& getWorldToClipTransform();

    protected:
        virtual void updateTransform() = 0;

    protected:
        glm::mat4 m_worldToCamera;
        glm::mat4 m_cameraToClip;
        glm::mat4 m_worldToClip;
        bool m_transformDirty{ true };

    };
}
