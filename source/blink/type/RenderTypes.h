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

    struct RenderDataGeo
    {
        glm::vec3 pos;
        glm::quat rot;
        Geometry* geometry;
    };

    struct RenderDataLight
    {
        glm::vec3 pos;
        glm::vec3 color;
        float intensity;
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
        alignas(16) glm::vec3 lightPos;
        alignas(16) glm::vec4 lightColorAndIntensity;
    };

    struct PerInstanceUniforms
    {
        alignas(16) glm::mat4 matLocalToWorld;
        alignas(16) glm::mat3x4 matLocalToWorldInvT;
    };

} // namespace blink
