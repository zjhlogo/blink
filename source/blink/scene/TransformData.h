/*!
 * \file TransformData.h
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 * 
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <foundation/BaseTypesGlm.h>

NS_BEGIN

struct TransformData
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

struct HierarchyData
{
    explicit HierarchyData(int _id, int parent)
        : id(_id)
        , parentId(parent)
    {

    };

    int id;
    int parentId;
};

NS_END
