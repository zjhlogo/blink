#pragma once
#include <materials/Material.h>

class NormalMappingMaterial : public blink::Material
{
public:
    RTTI_DEF(NormalMappingMaterial, blink::Material);

    NormalMappingMaterial();

    virtual void setupShaderUniforms(blink::Shader* shader) override;

protected:
    virtual ~NormalMappingMaterial();

};
