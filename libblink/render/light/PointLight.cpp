#include "PointLight.h"
#include "../shader/Shader.h"

namespace blink
{
    void PointLight::setupShaderUniforms(Shader * shader, const TransformData& transform)
    {
        shader->setUniform("u_lightPos", transform.position);
        shader->setUniform("u_lightColor", getLightColor());
    }
}
