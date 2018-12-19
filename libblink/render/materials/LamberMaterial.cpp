#include "LamberMaterial.h"

namespace blink
{
    LamberMaterial::LamberMaterial()
    {
        m_shader = Shader::fromStock(Shader::StockShaders::Lamber);
    }

    LamberMaterial::~LamberMaterial()
    {
        SAFE_RELEASE(m_shader);
    }
}
