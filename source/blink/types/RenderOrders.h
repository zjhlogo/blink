
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
        static const uint32 STATIC_OPAQUE = 1000;
        static const uint32 DYNAMIC_OPAQUE = 2000;
        static const uint32 TRANSPARENT = 3000;
    };
} // namespace blink
