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

    struct Position
    {
        glm::vec3 value;
    };

    struct Rotation
    {
        glm::quat value;
    };

    struct StaticModel
    {
        tstring geometryId;
        tstring materialId;
    };

    struct CameraData
    {
        glm::vec3 targetPos;
        glm::vec3 up;
        float fov;
        float aspect;
        float near;
        float far;
    };

    struct RenderFeature
    {
        int order;
        tstring materialId;
    };
} // namespace blink
