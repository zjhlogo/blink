#pragma once
#include "Object.h"
#include "../geometries/Geometry.h"
#include "../materials/Material.h"

namespace blink
{
    class Mesh : public Object
    {
    public:
        Mesh(Geometry* geometry, Material* material);
        ~Mesh();

    private:
        Geometry* m_geometry{};
        Material* m_material{};

    };
}
