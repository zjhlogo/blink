/*!
 * \file WireframeMaterial.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once
#include "Material.h"

NS_BEGIN

class WireframeMaterial : public Material
{
public:
    WireframeMaterial();
    virtual ~WireframeMaterial();

    virtual void setupShaderUniforms(Shader* shader) override;

};

NS_END
