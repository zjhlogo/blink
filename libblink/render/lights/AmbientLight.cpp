#include "AmbientLight.h"
#include "../shaders/Shader.h"

namespace blink
{
    AmbientLight::AmbientLight()
    {
        setLightColor(VEC3_ZERO);
    }

    AmbientLight::~AmbientLight()
    {

    }

    void AmbientLight::setupShaderUniforms(Shader * shader)
    {
        shader->setUniform("u_ambientColor", getLightColor());
    }
}
