#include "DiffuseMaterial.h"
#include <render/shader/Shader.h>

const char* MAP_TILE_VS =
#include "Diffuse.vs"
;

const char* MAP_TILE_FS =
#include "Diffuse.fs"
;

DiffuseMaterial::DiffuseMaterial()
{
}

DiffuseMaterial::~DiffuseMaterial()
{
}

std::shared_ptr<NS::Shader> DiffuseMaterial::getShader()
{
    if (!m_shader)
    {
        m_shader = NS::Shader::fromBuffer("map_tile", MAP_TILE_VS, nullptr, MAP_TILE_FS);
    }
    return m_shader;
}

void DiffuseMaterial::setupShaderUniforms(std::shared_ptr<NS::Shader> shader)
{

}
