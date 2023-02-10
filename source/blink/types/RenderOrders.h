/*********************************************************************
 * \file   RenderOrders.h
 * \brief
 *
 * \author zjhlogo
 * \date   4/15 2022
 *********************************************************************/
#pragma once

#include <foundation/BaseTypes.h>

namespace blink
{
    class RenderOrders
    {
    public:
        static constexpr uint32_t STATIC_OPAQUE = 1000;
        static constexpr uint32_t DYNAMIC_OPAQUE = 2000;
        static constexpr uint32_t TRANSPARENT = 3000;
    };
} // namespace blink
