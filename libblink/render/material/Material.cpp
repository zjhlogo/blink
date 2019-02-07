#include "Material.h"
#include "../shader/Shader.h"
#include "../texture/Texture2D.h"

namespace blink
{
    Material::Material()
    {
        for (int i = 0; i < MAX_TEXTURES; ++i)
        {
            m_texInfos[i].index = i;
        }
    }

    Material::~Material()
    {
        for (int i = 0; i < MAX_TEXTURES; ++i)
        {
            SAFE_RELEASE(m_texInfos[i].texture);
            m_texInfos[i].name.clear();
        }

        SAFE_RELEASE(m_shader);
    }

    bool Material::setTexture(const tstring& name, const tstring& filePath, uint32 index)
    {
        auto texture = Texture2D::fromFile(filePath);
        if (!texture) return false;

        if (m_texInfos[index].texture)
        {
            SAFE_RELEASE(m_texInfos[index].texture);
            m_texInfos[index].name.clear();
        }

        m_texInfos[index].name = name;
        m_texInfos[index].texture = texture;

        return true;
    }

    const Texture * Material::getTexture(uint32 index)
    {
        if (index < 0 || index >= MAX_TEXTURES) return nullptr;

        return m_texInfos[index].texture;
    }

    const Texture * Material::getTexture(const tstring & name)
    {
        for (int i = 0; i < MAX_TEXTURES; ++i)
        {
            if (m_texInfos[i].name == name)
            {
                return m_texInfos[i].texture;
            }
        }

        return nullptr;
    }

    void Material::setupShaderSampler(Shader * shader)
    {
        for (int i = 0; i < MAX_TEXTURES; ++i)
        {
            if (m_texInfos[i].texture)
            {
                shader->setTexture(m_texInfos[i].name.c_str(), m_texInfos[i].texture, m_texInfos[i].index);
            }
        }
    }

    void Material::release()
    {
        if (decReference() <= 0)
        {
            delete this;
        }
    }
}
