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

        void setLightColor(const glm::vec3& color) { m_lightColor = color; };
        const glm::vec3& getLightColor() const { return m_lightColor; };

        virtual void setupShaderUniforms(Shader* shader, const TransformData& transform) = 0;

        void release();
        int incReference() { return ++m_reference; };
        int decReference() { return --m_reference; };

    protected:
        Light() {};
        virtual ~Light() {};

    private:
        glm::vec3 m_lightColor{ VEC3_ONE };
        int m_reference{};

    };
}
