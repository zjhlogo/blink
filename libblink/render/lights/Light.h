#pragma once
#include "../objects/Object.h"

namespace blink
{
    class Light : public Object
    {
    public:
        RTTI_DEF(Light, Object);

        void setLightColor(const glm::vec3& color) { m_lightColor = color; };
        const glm::vec3& getLightColor() const { return m_lightColor; };

    private:
        glm::vec3 m_lightColor{ VEC3_ONE };

    };
}