/*********************************************************************
 * \file   VertexAttrs.h
 * \brief
 *
 * \author
 * zjhlogo
 * \date   04/22/2022
 *********************************************************************/
#pragma once
#include <bitflags/bitflags.hpp>

namespace blink
{
    BEGIN_RAW_BITFLAGS(VertexAttrs)
    RAW_FLAG(None)
    RAW_FLAG(Position)
    RAW_FLAG(Normal)
    RAW_FLAG(Uv0)
    RAW_FLAG(Uv1)
    RAW_FLAG(Uv2)
    RAW_FLAG(Uv3)
    RAW_FLAG(Uv4)
    RAW_FLAG(Uv5)
    RAW_FLAG(Uv6)
    RAW_FLAG(Uv7)
    RAW_FLAG(Color)
    END_RAW_BITFLAGS(VertexAttrs)
} // namespace blink
