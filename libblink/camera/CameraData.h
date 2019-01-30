#pragma once
#include <BaseType.h>

namespace blink
{
	struct CameraData
    {
        enum BitFlag
        {
            BF_DIRTY = 1 << 0,
        };

        CameraData(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up) : cameraPos(pos), cameraTarget(target), cameraUp(up) {};

        glm::vec3 cameraPos;
        glm::vec3 cameraTarget;
        glm::vec3 cameraUp;
		
        glm::mat4 worldToCamera;
        glm::mat4 cameraToClip;
        glm::mat4 worldToClip;

        uint32 bitFlag{ BF_DIRTY };
    };
}
