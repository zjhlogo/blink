#pragma once
#include <render/material/Material.h>

class DiffuseMaterial : public NS::Material
{
public:
    DiffuseMaterial();
    virtual ~DiffuseMaterial();

    virtual std::shared_ptr<NS::Shader> getShader() override;
    virtual void setupShaderUniforms(std::shared_ptr<NS::Shader> shader) override;

};
