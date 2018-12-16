#include "Texture2D.h"
#include "../../utils/ImageLoader.h"
#include <Log.h>
#include <glad/glad.h>

namespace blink
{
    Texture2D::Texture2D()
    {

    }

    Texture2D::~Texture2D()
    {
        destroyTexture();
    }

    bool Texture2D::loadFromFile(const tstring& filePath)
    {
        ImageLoader::ImageInfo imageInfo;
        if (!ImageLoader::loadTextureFromImage(imageInfo, filePath))
        {
            LOGE("Load Texture2D failed '{0}'", filePath);
            return false;
        }

        destroyTexture();
        return updateTextureData(imageInfo.width, imageInfo.height, imageInfo.channels, imageInfo.data);
    }

    bool Texture2D::updateTextureData(int width, int height, int channel, const void* data)
    {
        ImageLoader::TextureInfo textureInfo;
        if (!ImageLoader::createTextureFromRawData(textureInfo, width, height, channel, data))
        {
            LOGE("Create Texture2D failed");
            return false;
        }

        m_textureId = textureInfo.textureId;
        m_textureSize = textureInfo.texSize;

        setMinFilter(m_minFilter);
        setMagFilter(m_magFilter);
        setUWrap(m_uWrap);
        setVWrap(m_vWrap);

        return true;
    }

    void Texture2D::setMinFilter(SampleFilter minFilter)
    {
        m_minFilter = minFilter;

        if (m_textureId != 0)
        {
            glBindTexture(GL_TEXTURE_2D, m_textureId);

            GLint glSampleFilter = GL_NEAREST;
            if (m_minFilter == SampleFilter::Linear) glSampleFilter = GL_LINEAR;
            else if (m_minFilter == SampleFilter::LinearMipmapLinear) glSampleFilter = GL_LINEAR_MIPMAP_LINEAR;
            else if (m_minFilter == SampleFilter::NearestMipmapNearest) glSampleFilter = GL_NEAREST_MIPMAP_NEAREST;
            else if (m_minFilter == SampleFilter::LinearMipmapNearest) glSampleFilter = GL_LINEAR_MIPMAP_NEAREST;
            else if (m_minFilter == SampleFilter::NearestMipmapLinear) glSampleFilter = GL_NEAREST_MIPMAP_LINEAR;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glSampleFilter);
        }
    }

    void Texture2D::setMagFilter(SampleFilter magFilter)
    {
        m_magFilter = magFilter;

        if (m_textureId != 0)
        {
            glBindTexture(GL_TEXTURE_2D, m_textureId);

            GLint glSampleFilter = GL_NEAREST;
            if (m_magFilter == SampleFilter::Linear) glSampleFilter = GL_LINEAR;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glSampleFilter);
        }
    }

    void Texture2D::setUWrap(WrapMethod uWrap)
    {
        m_uWrap = uWrap;

        if (m_textureId != 0)
        {
            glBindTexture(GL_TEXTURE_2D, m_textureId);

            GLint wrapParam = GL_CLAMP_TO_EDGE;
            if (m_uWrap == WrapMethod::Repeat) wrapParam = GL_REPEAT;
            else if (m_uWrap == WrapMethod::MirrorRepeat) wrapParam = GL_MIRRORED_REPEAT;

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapParam);
        }
    }

    void Texture2D::setVWrap(WrapMethod vWrap)
    {
        m_vWrap = vWrap;

        if (m_textureId != 0)
        {
            glBindTexture(GL_TEXTURE_2D, m_textureId);

            GLint wrapParam = GL_CLAMP_TO_EDGE;
            if (m_vWrap == WrapMethod::Repeat) wrapParam = GL_REPEAT;
            else if (m_vWrap == WrapMethod::MirrorRepeat) wrapParam = GL_MIRRORED_REPEAT;

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapParam);
        }
    }

    void Texture2D::destroyTexture()
    {
        if (m_textureId != 0)
        {
            glDeleteTextures(1, &m_textureId);
            m_textureId = 0;
        }
    }
}
