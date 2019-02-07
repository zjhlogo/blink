#pragma once
#include "Material.h"

namespace blink
{
    class WireframeMaterial : public Material
    {
    public:
        RTTI_DEF(WireframeMaterial, Material);

        virtual void setupShaderUniforms(Shader* shader) override;

    protected:
        WireframeMaterial();
        virtual ~WireframeMaterial();

    };
}
