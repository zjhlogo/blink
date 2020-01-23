/*!
 * \file AmbientLight.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "AmbientLight.h"
#include <render_base/Shader.h>

NS_BEGIN

AmbientLight::AmbientLight()
{
    setLightColor(VEC3_ZERO);
}

AmbientLight::~AmbientLight()
{

}

void AmbientLight::setupShaderUniforms(std::shared_ptr<Shader> shader, const TransformData& transform)
{
    shader->setUniform("u_ambientColor", getLightColor());
}

NS_END
