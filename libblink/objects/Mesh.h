#pragma once
#include "Object.h"
#include "../geometries/Geometry.h"
#include "../materials/Material.h"

namespace blink
{
    class Mesh : public Object
    {
    public:
        RTTI_DEF(Mesh, Object);

        Mesh(Geometry* geometry, Material* material);
        ~Mesh();

        Geometry* getGeometry() const { return m_geometry; }
        Material* getMaterial() const { return m_material; }

    protected:
        Geometry* m_geometry{};
        Material* m_material{};

    };
}
