#pragma once
#include "Material.h"

namespace blink
{
    class PhongMaterial : public Material
    {
    public:
        RTTI_DEF(PhongMaterial, Material);

        PhongMaterial();

        virtual Shader* getShader() override;
        virtual void setupShaderUniforms(Shader* shader) override;

    protected:
        virtual ~PhongMaterial();

    };
}
