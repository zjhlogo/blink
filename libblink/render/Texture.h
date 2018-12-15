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
        bool loadFromFile(const tstring& filePath);
        virtual bool reload();

        void setMinFilter(SampleFilter minFilter);
        SampleFilter getMinFilter() const { return m_minFilter; };

        void setMagFilter(SampleFilter magFilter);
        SampleFilter getMagFilter() const { return m_magFilter; };

        void setUWrap(WrapMethod uWrap);
        WrapMethod getUWrap() const { return m_uWrap; };

        void setVWrap(WrapMethod vWrap);
        WrapMethod getVWrap() const { return m_vWrap; };

        uint32 getTextureId() const { return m_textureId; }

        const glm::ivec2& getTextureSize() const { return m_textureSize; }

        bool updateTextureData(int width, int height, int channel, const BufferData& buffer);

    protected:
        Texture();
        ~Texture();

        void destroyTexture();

    private:
        uint32 m_textureId{};
        glm::ivec2 m_textureSize;

        SampleFilter m_minFilter{ SampleFilter::Nearest };
        SampleFilter m_magFilter{ SampleFilter::Nearest };

        WrapMethod m_uWrap{ WrapMethod::Clamp };
        WrapMethod m_vWrap{ WrapMethod::Clamp };

    };
}
