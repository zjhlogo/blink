#pragma once

#include <materials/Material.h>

class CartoonMaterial : public blink::Material
{
public:
    RTTI_DEF(CartoonMaterial, blink::Material);

    CartoonMaterial();

    virtual void setupShaderUniforms(blink::Shader* shader) override;

protected:
    virtual ~CartoonMaterial();

};
