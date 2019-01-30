#pragma once
#include <BaseType.h>
#include <glm/gtc/quaternion.hpp>

namespace blink
{
    struct Transform
    {
        enum BitFlag
        {
            BF_DIRTY = 1 << 0,
        };

        glm::vec3 position{ VEC3_ZERO };
        glm::quat rotation{ QUAT_ZERO };
        glm::vec3 scale{ VEC3_ONE };
        glm::mat4 localToParentTransform;
        glm::mat4 localToWorldTransform;

        uint32 bitFlag{ BF_DIRTY };
    };

    struct Hierarchy
    {
        Hierarchy(int id_, int parentId_) : id(id_), parentId(parentId_) {};

        int id;
        int parentId;
    };
}
