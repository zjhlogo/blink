/**

    @file      Components.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include "../type/RenderLayers.h"
#include "../type/RenderOrders.h"

#include <core/resources/IGeometry.h>
#include <core/resources/IMaterial.h>

namespace blink
{
    struct StaticModel
    {
        IGeometry* geometry;
        IMaterial* material;
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

    struct Renderable
    {
        uint32 renderLayer; // see RenderLayers
    };

    struct RenderFeature
    {
        int order;
        uint32 renderLayer; // see RenderLayers
        IMaterial* overrideMaterial;
    };
} // namespace blink
