#pragma once
#include <glm/glm.hpp>

namespace blink
{
    struct PushConstantData
    {
        glm::mat4 localToWorld;
        glm::mat3x4 localToWorldInvT;
    };
}
