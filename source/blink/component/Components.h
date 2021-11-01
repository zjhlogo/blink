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
    class Mesh;
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
        Mesh* mesh;
        Material* material;
    };

} // namespace blink
