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

            if (getTexture("tex_diffuse")) defineFlag |= Shader::USE_DIFFUSE_TEXTURE;

            m_shader = Shader::fromStock(Shader::StockShaders::Phong, defineFlag);
        }
        return m_shader;
    }

    void LambertMaterial::setupShaderUniforms(Shader * shader)
    {
        shader->setUniform("u_diffuseColor", getDiffuseColor());
    }
}
