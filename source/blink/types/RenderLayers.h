/*********************************************************************
 * \file   RenderLayers.h
 * \brief
 *
 * \author zjhlogo
 * \date   4/15 2022
 *********************************************************************/
#pragma once

#include <foundation/BaseTypes.h>

namespace blink
{
    class RenderLayers
    {
    public:
        static constexpr uint32_t NONE = 0;
        static constexpr uint32_t DEBUG = 1 << 0;
        static constexpr uint32_t STATIC = 1 << 1;
        static constexpr uint32_t NORMAL = 1 << 2;
        static constexpr uint32_t ALL = STATIC | NORMAL;
    };
} // namespace blink
