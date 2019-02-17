#include "PointLight.h"
#include "../shader/Shader.h"

namespace blink
{
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
}
