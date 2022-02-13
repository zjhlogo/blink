/**

    @file      Components.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <foundation/BaseTypesGlm.h>

namespace blink
{
    class Geometry;
    class Material;

    struct StaticModel
    {
        Geometry* geometry;
        Material* material;
    };

    struct CameraData
    {
        float fov;
        float aspect;
        float near;
        float far;
    };

    struct LightData
    {
        glm::vec3 color;
        float intensity;
    };

    struct RenderFeature
    {
        int order;
        Material* material;
    };
} // namespace blink
