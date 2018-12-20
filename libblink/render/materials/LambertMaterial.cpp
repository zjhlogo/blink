#include "LambertMaterial.h"

namespace blink
{
    LambertMaterial::LambertMaterial()
    {
        m_shader = Shader::fromStock(Shader::StockShaders::Lambert);
    }

    LambertMaterial::~LambertMaterial()
    {

    }
}
