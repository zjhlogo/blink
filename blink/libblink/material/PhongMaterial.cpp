#include "PhongMaterial.h"
#include "../shader/Shader.h"

namespace blink
{
    PhongMaterial::PhongMaterial()
    {

    }

    PhongMaterial::~PhongMaterial()
    {

    }

    std::shared_ptr<Shader> PhongMaterial::getShader()
    {
        if (!m_shader)
        {
            uint32 defineFlag = 0;

            if (getTexture("tex_diffuse")) defineFlag |= Shader::USE_DIFFUSE_TEXTURE;

            m_shader = Shader::fromStock(Shader::StockShaders::Phong, defineFlag);
        }
        return m_shader;
    }

    void PhongMaterial::setupShaderUniforms(std::shared_ptr<Shader> shader)
    {
        shader->setUniform("u_diffuseColor", getDiffuseColor());
    }
}
