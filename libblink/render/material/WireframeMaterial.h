#pragma once
#include "Material.h"

namespace blink
{
    class WireframeMaterial : public Material
    {
    public:
        RTTI_DEF(WireframeMaterial, Material);
        WireframeMaterial();
        virtual ~WireframeMaterial();

        virtual void setupShaderUniforms(std::shared_ptr<Shader> shader) override;

    };
}
