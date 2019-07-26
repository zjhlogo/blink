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

    }

    std::shared_ptr<Texture> Material::setTexture(const tstring& name, const tstring& filePath, uint32 index)
    {
        return setTexture(name, Texture2D::fromFile(filePath), index);
    }

    std::shared_ptr<Texture> Material::setTexture(const tstring & name, std::shared_ptr<Texture> texture, uint32 index)
    {
        if (!texture) return nullptr;

        if (m_texInfos[index].texture)
        {
            m_texInfos[index].texture = nullptr;
            m_texInfos[index].name.clear();
        }

        m_texInfos[index].name = name;
        m_texInfos[index].texture = texture;

        return texture;
    }

    std::shared_ptr<Texture> Material::getTexture(uint32 index)
    {
        if (index < 0 || index >= MAX_TEXTURES) return nullptr;

        return m_texInfos[index].texture;
    }

    std::shared_ptr<Texture> Material::getTexture(const tstring & name)
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

    void Material::setupShaderSampler(std::shared_ptr<Shader> shader)
    {
        for (int i = 0; i < MAX_TEXTURES; ++i)
        {
            if (m_texInfos[i].texture)
            {
                shader->setTexture(m_texInfos[i].name.c_str(), m_texInfos[i].texture, m_texInfos[i].index);
            }
        }
    }
}
