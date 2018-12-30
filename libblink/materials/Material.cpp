#include "Material.h"
#include "../shaders/Shader.h"
#include "../textures/Texture2D.h"

namespace blink
{
    Material::Material()
    {
    }

    Material::~Material()
    {
        for (auto& texInfo : m_texInfoMap)
        {
            SAFE_RELEASE(texInfo.second.texture);
        }
        m_texInfoMap.clear();

        SAFE_RELEASE(m_shader);
    }

    bool Material::setTexture(const tstring& name, const tstring& filePath, uint32 index)
    {
        auto texture = Texture2D::fromFile(filePath);
        if (!texture) return false;

        m_texInfoMap[index] = { name, texture, index };
        return true;
    }

    const Texture * Material::getTexture(uint32 index)
    {
        auto it = m_texInfoMap.find(index);
        if (it == m_texInfoMap.end()) return nullptr;

        return it->second.texture;
    }

    const Texture * Material::getTexture(const tstring & name)
    {
        for (const auto& texInfo : m_texInfoMap)
        {
            if (texInfo.second.name == name)
            {
                return texInfo.second.texture;
            }
        }

        return nullptr;
    }

    void Material::setupShaderSampler(Shader * shader)
    {
        for (const auto& texInfo : m_texInfoMap)
        {
            shader->setTexture(texInfo.second.name.c_str(), texInfo.second.texture, texInfo.first);
        }
    }
}
