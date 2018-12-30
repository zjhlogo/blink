#pragma once
#include "Material.h"

namespace blink
{
    class WireframeMaterial : public Material
    {
    public:
        RTTI_DEF(WireframeMaterial, Material);

        WireframeMaterial();

        virtual void setupShaderUniforms(Shader* shader) override;

    protected:
        virtual ~WireframeMaterial();

    };
}
