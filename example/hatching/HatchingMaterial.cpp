#include "HatchingMaterial.h"
#include <shaders/Shader.h>

static const char* HATCHING_VS =
#include "Hatching.vs"
;

static const char* HATCHING_FS =
#include "Hatching.fs"
;

HatchingMaterial::HatchingMaterial()
{
    m_shader = blink::Shader::fromBuffer("hatching", HATCHING_VS, nullptr, HATCHING_FS);
}

HatchingMaterial::~HatchingMaterial()
{
    SAFE_RELEASE(m_shader);
}

void HatchingMaterial::setupShaderUniforms(blink::Shader * shader)
{
    shader->setUniform("u_diffuseColor", getDiffuseColor());
}
