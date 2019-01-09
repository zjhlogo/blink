#include "SpriteMaterial.h"
#include <shaders/Shader.h>

static const char* SPRITE_VS =
#include "Sprite.vs"
;

static const char* SPRITE_FS =
#include "Sprite.fs"
;

SpriteMaterial::SpriteMaterial()
{
    m_shader = blink::Shader::fromBuffer("sprite", SPRITE_VS, nullptr, SPRITE_FS);
}

void SpriteMaterial::setupShaderUniforms(blink::Shader * shader)
{
    // TODO: 
}
