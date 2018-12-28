#include "LambertMaterial.h"
#include "../shaders/Shader.h"

namespace blink
{
    LambertMaterial::LambertMaterial()
    {

    }

    LambertMaterial::~LambertMaterial()
    {

    }

    Shader * LambertMaterial::getShader()
    {
        if (!m_shader)
        {
            uint32 defineFlag = 0;
            if (m_textureMap.find(TextureType::Diffuse) != m_textureMap.end()) defineFlag |= Shader::USE_DIFFUSE_TEXTURE;

            m_shader = Shader::fromStock(Shader::StockShaders::Lambert, defineFlag);
        }
        return m_shader;
    }

    void LambertMaterial::setupShaderUniforms(Shader * shader)
    {
        shader->setUniform("u_diffuseColor", getDiffuseColor());
    }
}
