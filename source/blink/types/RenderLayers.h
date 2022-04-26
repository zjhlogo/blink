
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
        static const uint32 NONE = 0;
        static const uint32 DEBUG = 1 << 0;
        static const uint32 STATIC = 1 << 1;
        static const uint32 NORMAL = 1 << 2;
        static const uint32 ALL = STATIC | NORMAL;
    };
} // namespace blink
