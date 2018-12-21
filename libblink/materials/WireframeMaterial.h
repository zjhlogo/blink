#pragma once
#include "Material.h"

namespace blink
{
    class WireframeMaterial : public Material
    {
    public:
        WireframeMaterial();
        virtual ~WireframeMaterial();

        virtual void setupShaderUniforms(Shader* shader) override;

    };
}
