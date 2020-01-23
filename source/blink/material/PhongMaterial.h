/*!
 * \file PhongMaterial.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once
#include "Material.h"

NS_BEGIN

class PhongMaterial : public Material
{
public:
    PhongMaterial();
    virtual ~PhongMaterial();

    virtual Shader* getShader() override;
    virtual void setupShaderUniforms(Shader* shader) override;

};

NS_END
