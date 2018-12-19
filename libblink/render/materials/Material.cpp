#include "Material.h"

namespace blink
{
    Material::Material()
    {
    }

    Material::~Material()
    {
        SAFE_RELEASE(m_shader);
    }
}
