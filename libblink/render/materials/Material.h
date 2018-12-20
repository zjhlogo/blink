#pragma once
#include <Rtti.h>
#include "Shader.h"

namespace blink
{
    class Material
    {
    public:
        RTTI_ROOT(Material);

        Material();
        virtual ~Material();

        Shader* getShader() const { return m_shader; };

        void setDiffuseColor(const glm::vec3& diffuseColor) { m_diffuseColor = diffuseColor; }
        const glm::vec3& getDiffuseColor() const { return m_diffuseColor; }

    protected:
        Shader* m_shader{};
        glm::vec3 m_diffuseColor{ VEC3_ONE };
        float m_specularStrength;

    };
}
