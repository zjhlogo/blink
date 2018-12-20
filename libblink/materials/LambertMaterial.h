#pragma once
#include "Material.h"

namespace blink
{
    class LambertMaterial : public Material
    {
    public:
        RTTI_DEF(LambertMaterial, Material);

        LambertMaterial();
        virtual ~LambertMaterial();

        virtual void setupShaderUniforms(Shader* shader) override;

    };
}
