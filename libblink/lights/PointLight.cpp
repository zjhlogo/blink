#include "PointLight.h"
#include "../shaders/Shader.h"

namespace blink
{
    void PointLight::setupShaderUniforms(Shader * shader)
    {
        shader->setUniform("u_lightPos", getPosition());
        shader->setUniform("u_lightColor", getLightColor());
    }
}
