/**

    @file      BuiltinFormatter.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      8.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include "BaseTypesGlm.h"

#include <fmt/format.h>

template <> struct fmt::formatter<glm::vec2>
{
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.end();
    }

    template <typename FormatContext> auto format(const glm::vec2& v, FormatContext& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "({:.1f}, {:.1f})", v.x, v.y);
    }
};

template <> struct fmt::formatter<glm::vec3>
{
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.end();
    }

    template <typename FormatContext> auto format(const glm::vec3& v, FormatContext& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "({:.1f}, {:.1f}, {:.1f})", v.x, v.y, v.z);
    }
};

template <> struct fmt::formatter<glm::vec4>
{
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.end();
    }

    template <typename FormatContext> auto format(const glm::vec4& v, FormatContext& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "({:.1f}, {:.1f}, {:.1f}, {:.1f})", v.x, v.y, v.z, v.w);
    }
};

template <> struct fmt::formatter<glm::quat>
{
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.end();
    }

    template <typename FormatContext> auto format(const glm::quat& v, FormatContext& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "({:.1f}, {:.1f}, {:.1f}, {:.1f})", v.x, v.y, v.z, v.w);
    }
};

template <> struct fmt::formatter<glm::u16vec2>
{
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.end();
    }

    template <typename FormatContext> auto format(const glm::u16vec2& v, FormatContext& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "({:d}, {:d})", v.x, v.y);
    }
};

template <> struct fmt::formatter<glm::u16vec3>
{
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.end();
    }

    template <typename FormatContext> auto format(const glm::u16vec3& v, FormatContext& ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "({:d}, {:d}, {:d})", v.x, v.y, v.z);
    }
};
