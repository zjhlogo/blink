#include "NormalMappingMaterial.h"
#include <shaders/Shader.h>

static const char* NORMAL_MAPPING_VS =
#include "NormalMapping.vs"
;

static const char* NORMAL_MAPPING_FS =
#include "NormalMapping.fs"
;

NormalMappingMaterial::NormalMappingMaterial()
{
    m_shader = blink::Shader::fromBuffer("normal_mapping", NORMAL_MAPPING_VS, nullptr, NORMAL_MAPPING_FS);
}

NormalMappingMaterial::~NormalMappingMaterial()
{
    SAFE_RELEASE(m_shader);
}

void NormalMappingMaterial::setupShaderUniforms(blink::Shader * shader)
{
    shader->setUniform("u_diffuseColor", getDiffuseColor());
}
