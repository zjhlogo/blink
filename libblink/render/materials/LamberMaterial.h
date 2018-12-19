#pragma once
#include "Material.h"
#include "Shader.h"

namespace blink
{
    class LamberMaterial : public Material
    {
    public:
        RTTI_DEF(LamberMaterial, Material);

        LamberMaterial();
        virtual ~LamberMaterial();

        Shader* getShader() const { return m_shader; };

    private:
        Shader* m_shader{};
    };
}
