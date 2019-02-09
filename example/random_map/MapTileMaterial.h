#pragma once
#include <render/material/Material.h>

class MapTileMaterial : public blink::Material
{
public:
    RTTI_DEF(MapTileMaterial, blink::Material);

    MapTileMaterial();

    virtual blink::Shader* getShader() override;
    virtual void setupShaderUniforms(blink::Shader* shader) override;

protected:
    virtual ~MapTileMaterial();

};
