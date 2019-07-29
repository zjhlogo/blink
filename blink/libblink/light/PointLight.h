/*!
 * \file PointLight.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once

#include "Light.h"

namespace blink
{
    class PointLight : public Light
    {
    public:
        PointLight();
        virtual ~PointLight();

        virtual void setupShaderUniforms(std::shared_ptr<Shader> shader, const TransformData& transform) override;

    };
}