#include "MapTileMaterial.h"
#include <render/shader/Shader.h>

const char* MAP_TILE_VS =
#include "MapTile.vs"
;

const char* MAP_TILE_FS =
#include "MapTile.fs"
;

MapTileMaterial::MapTileMaterial()
{
}

MapTileMaterial::~MapTileMaterial()
{
}

std::shared_ptr<blink::Shader> MapTileMaterial::getShader()
{
    if (!m_shader)
    {
        m_shader = blink::Shader::fromBuffer("map_tile", MAP_TILE_VS, nullptr, MAP_TILE_FS);
    }
    return m_shader;
}

void MapTileMaterial::setupShaderUniforms(std::shared_ptr<blink::Shader> shader)
{

}
