#include "WireframeMaterial.h"
#include "../shaders/Shader.h"

namespace blink
{
    WireframeMaterial::WireframeMaterial()
    {
        m_shader = Shader::fromStock(Shader::StockShaders::Wireframe, 0);
    }

    WireframeMaterial::~WireframeMaterial()
    {
    }

    void WireframeMaterial::setupShaderUniforms(Shader * shader)
    {
        shader->setUniform("u_diffuseColor", getDiffuseColor());
    }
}
