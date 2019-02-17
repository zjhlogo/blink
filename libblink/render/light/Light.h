#pragma once
#include <Rtti.h>
#include "../../scene/TransformData.h"

namespace blink
{
    class Shader;

    class Light
    {
    public:
        RTTI_ROOT(Light);
        Light();
        virtual ~Light();

        void setLightColor(const glm::vec3& color) { m_lightColor = color; };
        const glm::vec3& getLightColor() const { return m_lightColor; };

        virtual void setupShaderUniforms(std::shared_ptr<Shader> shader, const TransformData& transform) = 0;

    private:
        glm::vec3 m_lightColor{ VEC3_ONE };

    };
}
