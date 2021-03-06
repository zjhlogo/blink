#include "Texture2D.h"
#include "../util/ImageLoader.h"
#include <Log.h>
#include <InstanceManager.h>
#include <glad/glad.h>

namespace blink
{
    static InstanceManager<tstring, Texture2D> s_instanceManager;

    Texture2D::Texture2D(const tstring& id)
        :Texture(id)
    {

    }

    Texture2D::~Texture2D()
    {
        destroyTexture();
    }

    std::shared_ptr<Texture2D> Texture2D::fromFile(const tstring& filePath)
    {
        auto exitInst = s_instanceManager.insertInstance("file::" + filePath);
        if (exitInst) return exitInst;

        ImageLoader::ImageInfo imageInfo;
        if (!ImageLoader::loadTextureFromImage(imageInfo, filePath, true))
        {
            LOGE("Load Texture2D failed '{0}'", filePath);
            return nullptr;
        }

        std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(filePath);
        if (!texture->updateTextureData(imageInfo.width, imageInfo.height, imageInfo.channels, imageInfo.data))
        {
            LOGE("Upload Texture2D data failed '{0}'", filePath);
            return nullptr;
        }

        return s_instanceManager.insertInstance("file::" + filePath, texture);
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
            else if (m_magFilter == SampleFilter::LinearMipmapLinear) glSampleFilter = GL_LINEAR_MIPMAP_LINEAR;
            else if (m_magFilter == SampleFilter::NearestMipmapNearest) glSampleFilter = GL_NEAREST_MIPMAP_NEAREST;
            else if (m_magFilter == SampleFilter::LinearMipmapNearest) glSampleFilter = GL_LINEAR_MIPMAP_NEAREST;
            else if (m_magFilter == SampleFilter::NearestMipmapLinear) glSampleFilter = GL_NEAREST_MIPMAP_LINEAR;
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
