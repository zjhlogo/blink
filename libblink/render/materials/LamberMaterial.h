#pragma once
#include "Material.h"

namespace blink
{
    class LamberMaterial : public Material
    {
    public:
        RTTI_DEF(LamberMaterial, Material);

        LamberMaterial();
        virtual ~LamberMaterial();

    };
}
