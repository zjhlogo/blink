#pragma once
#include <BaseType.h>

namespace blink
{
    class Texture
    {
    public:
        enum class SampleFilter
        {
            Nearest = 0,
            Linear,
            LinearMipmapLinear,
            NearestMipmapNearest,
            LinearMipmapNearest,
            NearestMipmapLinear,
        };

        enum class WrapMethod
        {
            Clamp = 0,
            Repeat,
            MirrorRepeat,
        };

    public:
        virtual uint32 getTextureId() const = 0;

    };
}
