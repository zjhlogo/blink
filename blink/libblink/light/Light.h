/*!
 * \file Light.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once
#include "../scene/TransformData.h"
#include <memory>

NS_BEGIN

class Shader;

class Light
{
public:
    Light();
    virtual ~Light();

    void setLightColor(const glm::vec3& color) { m_lightColor = color; };
    const glm::vec3& getLightColor() const { return m_lightColor; };

    virtual void setupShaderUniforms(std::shared_ptr<Shader> shader, const TransformData& transform) = 0;

private:
    glm::vec3 m_lightColor{ VEC3_ONE };

};

NS_END
