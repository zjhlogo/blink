#include "PointLight.h"
#include "../materials/Shader.h"

namespace blink
{
    void PointLight::setupShaderUniforms(Shader * shader)
    {
        shader->setUniform("u_lightPos", getPosition());
        shader->setUniform("u_lightColor", getLightColor());
    }
}
