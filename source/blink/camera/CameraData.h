/*!
 * \file CameraData.h
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

class Shader;

struct CameraData
{
    enum BitFlag
    {
        BF_DIRTY = 1 << 0,
    };

    explicit CameraData(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up)
        : cameraPos(pos)
        , cameraTarget(target)
        , cameraUp(up)
        , bitFlag(BF_DIRTY)
    {

    };

    glm::vec3 cameraPos;
    glm::vec3 cameraTarget;
    glm::vec3 cameraUp;

    glm::mat4 worldToCamera;
    glm::mat4 cameraToClip;
    glm::mat4 worldToClip;

    uint32 bitFlag;
};

NS_END
