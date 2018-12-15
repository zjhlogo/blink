#include "Texture.h"
#include <glad/glad.h>

namespace blink
{
    Texture::Texture()
    {

    }

    Texture::~Texture()
    {
        destroyTexture();
    }

    bool Texture::loadFromFile(const tstring& filePath)
    {
        m_imageFileType = ImageUtil::getImageFileType(filePath);
        if (m_imageFileType == ImageUtil::ImageFileType::Unknown)
        {
            LOGE("Unknown image type {0}", filePath);
            return false;
        }

        return reload();
    }

    bool Texture::reload()
    {
        destroyTexture();

        ImageUtil::ImageInfo imageInfo;
        if (!ImageUtil::loadTextureFromImage(imageInfo, m_imageFileType, filePath))
        {
            LOGE("Load texture failed '{0}'", filePath);
            return false;
        }

        return updateTextureData(imageInfo.m_width, imageInfo.m_height, imageInfo.m_channels, imageInfo.m_bufferData);
    }

    bool Texture::updateTextureData(int width, int height, int channel, const BufferData& buffer)
    {
        destroyTexture();

        ImageUtil::TextureInfo textureInfo;
        if (!ImageUtil::createTextureFromRawData(textureInfo, width, height, channel, buffer))
        {
            LOGE("Create texture failed '{0}'", filePath);
            return false;
        }

        m_textureId = textureInfo.m_textureId;
        m_textureSize = textureInfo.m_texSize;

        setMinFilter(m_minFilter);
        setMagFilter(m_magFilter);
        setUWrap(m_uWrap);
        setVWrap(m_vWrap);

        return true;
    }

    void Texture::setMinFilter(SampleFilter minFilter)
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

    void Texture::setMagFilter(SampleFilter magFilter)
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

    void Texture::setUWrap(WrapMethod uWrap)
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

    void Texture::setVWrap(WrapMethod vWrap)
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

    void Texture::destroyTexture()
    {
        if (m_textureId != 0)
        {
            glDeleteTextures(1, &m_textureId);
            m_textureId = 0;
        }
    }
}
