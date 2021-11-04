/**

    @file      IGeometryBuilder.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      4.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <foundation/BaseTypesGlm.h>

#include <vector>

namespace blink
{
    class IGeometryBuilder
    {
    public:
        virtual uint32 build(std::vector<glm::vec3>& positionsOut,
                             std::vector<uint16>& indicesOut,
                             std::vector<glm::vec3>* normalsOut = nullptr,
                             std::vector<glm::vec2>* uvsOut = nullptr) = 0;
    };
} // namespace blink
