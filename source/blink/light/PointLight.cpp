/*!
 * \file PointLight.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "PointLight.h"
#include <render_base/Shader.h>

NS_BEGIN

PointLight::PointLight()
{

}

PointLight::~PointLight()
{

}

void PointLight::setupShaderUniforms(std::shared_ptr<Shader> shader, const TransformData& transform)
{
    shader->setUniform("u_lightPos", transform.position);
    shader->setUniform("u_lightColor", getLightColor());
}

NS_END
