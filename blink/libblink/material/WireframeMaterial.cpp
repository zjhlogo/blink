#include "WireframeMaterial.h"
#include "../shader/Shader.h"

namespace blink
{
    WireframeMaterial::WireframeMaterial()
    {
        m_shader = Shader::fromStock(Shader::StockShaders::Wireframe, 0);
    }

    WireframeMaterial::~WireframeMaterial()
    {
    }

    void WireframeMaterial::setupShaderUniforms(std::shared_ptr<Shader> shader)
    {
        shader->setUniform("u_diffuseColor", getDiffuseColor());
    }
}
