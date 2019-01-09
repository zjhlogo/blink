#pragma once
#include <materials/Material.h>

class SpriteMaterial : public blink::Material
{
public:
    RTTI_DEF(SpriteMaterial, blink::Material);

    SpriteMaterial();

    virtual void setupShaderUniforms(blink::Shader* shader) override;

};