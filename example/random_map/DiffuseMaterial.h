#pragma once
#include <render/material/Material.h>

class DiffuseMaterial : public blink::Material
{
public:
    RTTI_DEF(DiffuseMaterial, blink::Material);

    DiffuseMaterial();
    virtual ~DiffuseMaterial();

    virtual std::shared_ptr<blink::Shader> getShader() override;
    virtual void setupShaderUniforms(std::shared_ptr<blink::Shader> shader) override;

};
