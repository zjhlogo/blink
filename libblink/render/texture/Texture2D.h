#pragma once
#include "Texture.h"
#include <memory>

namespace blink
{
    class Texture2D : public Texture
    {
    public:
        RTTI_DEF(Texture2D, Texture);

        Texture2D(const tstring& id);
        virtual ~Texture2D();

        static std::shared_ptr<Texture2D> fromFile(const tstring& filePath);

        void setMinFilter(SampleFilter minFilter);
        SampleFilter getMinFilter() const { return m_minFilter; };

        void setMagFilter(SampleFilter magFilter);
        SampleFilter getMagFilter() const { return m_magFilter; };

        void setUWrap(WrapMethod uWrap);
        WrapMethod getUWrap() const { return m_uWrap; };

        void setVWrap(WrapMethod vWrap);
        WrapMethod getVWrap() const { return m_vWrap; };

        uint32 getTextureId() const override { return m_textureId; }

        const glm::ivec2& getTextureSize() const { return m_textureSize; }
        bool updateTextureData(int width, int height, int channel, const void* data);

    protected:
        void destroyTexture();

    private:
        uint32 m_textureId{};
        glm::ivec2 m_textureSize;

        SampleFilter m_minFilter{ SampleFilter::LinearMipmapLinear };
        SampleFilter m_magFilter{ SampleFilter::Linear };

        WrapMethod m_uWrap{ WrapMethod::Clamp };
        WrapMethod m_vWrap{ WrapMethod::Clamp };

    };
}
