#pragma once
#include <BaseTypes.h>
#include <BaseTypesGlm.h>

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
        Texture(const tstring& id);
        virtual ~Texture();
        const tstring& getId() const { return m_id; };

        virtual uint32 getTextureId() const = 0;

    private:
        tstring m_id;

    };
}
