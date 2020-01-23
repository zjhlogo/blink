/*!
 * \file Material.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "Material.h"
#include <render_base/Shader.h>
#include <render_base/Texture.h>

NS_BEGIN

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

Texture* Material::setTexture(const tstring& name, const tstring& filePath, uint32 index)
{
    return nullptr;
// TODO:        return setTexture(name, Texture2D::fromFile(filePath), index);
}

Texture* Material::setTexture(const tstring & name, Texture* texture, uint32 index)
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

Texture* Material::getTexture(uint32 index)
{
    if (index < 0 || index >= MAX_TEXTURES) return nullptr;

    return m_texInfos[index].texture;
}

Texture* Material::getTexture(const tstring & name)
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

void Material::setupShaderSampler(Shader* shader)
{
    for (int i = 0; i < MAX_TEXTURES; ++i)
    {
        if (m_texInfos[i].texture)
        {
            shader->setTexture(m_texInfos[i].name.c_str(), m_texInfos[i].texture, m_texInfos[i].index);
        }
    }
}

NS_END
