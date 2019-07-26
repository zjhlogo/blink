#pragma once
#include <BaseType.h>
#include <Rtti.h>

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
        RTTI_ROOT(Texture);

        Texture(const tstring& id);
        virtual ~Texture();
        const tstring& getId() const { return m_id; };

        virtual uint32 getTextureId() const = 0;

    private:
        tstring m_id;

    };
}
