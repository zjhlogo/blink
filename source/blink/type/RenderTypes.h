/**

    @file      RenderTypes.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      7.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <foundation/BaseTypesGlm.h>

namespace blink
{
    class Geometry;
    class Material;

    struct RenderData
    {
        glm::vec3 pos;
        glm::quat rot;
        Geometry* geometry;
    };

    struct RenderFeatureData
    {
        int order;
        Material* material;
    };

    struct PerFrameUniforms
    {
        alignas(16) glm::mat4 matWorldToCamera;
        alignas(16) glm::mat4 matCameraToProjection;
        alignas(16) glm::mat4 matWorldToProjection;
        alignas(16) glm::mat3x4 matWorldToCameraInvT;
        alignas(16) glm::vec3 cameraPos;
        alignas(16) glm::vec3 cameraDir;
    };

    struct PerInstanceUniforms
    {
        alignas(16) glm::mat4 matLocalToWorld;
        alignas(16) glm::mat3x4 matLocalToWorldInvT;
    };

} // namespace blink
