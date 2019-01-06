#include "CartoonMaterial.h"
#include <shaders/Shader.h>

static const char* CARTOON_VS =
#include "Cartoon.vs"
;

static const char* CARTOON_FS =
#include "Cartoon.fs"
;

CartoonMaterial::CartoonMaterial()
{
    m_shader = blink::Shader::fromBuffer("cartoon", CARTOON_VS, nullptr, CARTOON_FS);
}

CartoonMaterial::~CartoonMaterial()
{
    SAFE_RELEASE(m_shader);
}

void CartoonMaterial::setupShaderUniforms(blink::Shader * shader)
{
    //shader->setUniform("u_diffuseColor", getDiffuseColor());
}
