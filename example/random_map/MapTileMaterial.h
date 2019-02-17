#pragma once
#include <render/material/Material.h>

class MapTileMaterial : public blink::Material
{
public:
    RTTI_DEF(MapTileMaterial, blink::Material);

    MapTileMaterial();
    virtual ~MapTileMaterial();

    virtual std::shared_ptr<blink::Shader> getShader() override;
    virtual void setupShaderUniforms(std::shared_ptr<blink::Shader> shader) override;

};
