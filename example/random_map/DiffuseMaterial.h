#pragma once
#include <render/material/Material.h>

class DiffuseMaterial : public blink::Material
{
public:
    DiffuseMaterial();
    virtual ~DiffuseMaterial();

    virtual std::shared_ptr<blink::Shader> getShader() override;
    virtual void setupShaderUniforms(std::shared_ptr<blink::Shader> shader) override;

};
