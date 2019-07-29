/*!
 * \file WireframeMaterial.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "WireframeMaterial.h"
#include <RenderModule.h>

namespace blink
{
    WireframeMaterial::WireframeMaterial()
    {
        m_shader = RenderModule::createShaderFromStock(Shader::StockShaders::Wireframe, 0);
    }

    WireframeMaterial::~WireframeMaterial()
    {
    }

    void WireframeMaterial::setupShaderUniforms(Shader* shader)
    {
        shader->setUniform("u_diffuseColor", getDiffuseColor());
    }
}
