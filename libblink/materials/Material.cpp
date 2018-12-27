#include "Material.h"
#include "../shaders/Shader.h"

namespace blink
{
    Material::Material()
    {
    }

    Material::~Material()
    {
        SAFE_RELEASE(m_shader);
    }

    void Material::setTexture(TextureType type, const Texture * texture)
    {
        m_textureMap[type] = texture;
    }

    const Texture * Material::getTexture(TextureType type)
    {
        return m_textureMap[type];
    }

    void Material::setupShaderSampler(Shader * shader)
    {
        static const char* s_textureNameList[static_cast<int>(TextureType::NumTextureType)] =
        {
            "tex_diffuse",
            "tex_normal",
        };

        for (const auto& texture : m_textureMap)
        {
            shader->setTexture(s_textureNameList[static_cast<int>(texture.first)], texture.second, static_cast<int>(texture.first));
        }
    }
}
