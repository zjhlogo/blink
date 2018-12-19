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

    protected:
        Shader* m_shader{};

    };
}
