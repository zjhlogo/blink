#include "LambertMaterial.h"
#include "../shaders/Shader.h"

namespace blink
{
    LambertMaterial::LambertMaterial()
    {
        m_shader = Shader::fromStock(Shader::StockShaders::Lambert);
    }

    LambertMaterial::~LambertMaterial()
    {

    }

    void LambertMaterial::setupShaderUniforms(Shader * shader)
    {
        shader->setUniform("u_diffuseColor", getDiffuseColor());
    }
}
