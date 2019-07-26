#include "AmbientLight.h"
#include "../shader/Shader.h"

namespace blink
{
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
}
