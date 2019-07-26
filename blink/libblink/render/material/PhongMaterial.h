#pragma once
#include "Material.h"

namespace blink
{
    class PhongMaterial : public Material
    {
    public:
        RTTI_DEF(PhongMaterial, Material);

        PhongMaterial();
        virtual ~PhongMaterial();

        std::shared_ptr<Shader> getShader() override;
        void setupShaderUniforms(std::shared_ptr<Shader> shader) override;

    };
}
