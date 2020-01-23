/*!
 * \file PhongMaterial.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#include "PhongMaterial.h"

#include <render_base/RenderModule.h>

NS_BEGIN

PhongMaterial::PhongMaterial()
{
}

PhongMaterial::~PhongMaterial()
{
}

Shader* PhongMaterial::getShader()
{
    if (!m_shader)
    {
        uint32 defineFlag = 0;

        if (getTexture("tex_diffuse")) defineFlag |= Shader::USE_DIFFUSE_TEXTURE;

        m_shader = RenderModule::getInstance().createShaderFromStock(Shader::StockShaders::Phong, defineFlag);
    }
    return m_shader;
}

void PhongMaterial::setupShaderUniforms(Shader* shader)
{
    shader->setUniform("u_diffuseColor", getDiffuseColor());
}

NS_END
