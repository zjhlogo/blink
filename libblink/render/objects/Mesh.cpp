#include "Mesh.h"

namespace blink
{
    Mesh::Mesh(Geometry * geometry, Material * material)
    {
        m_geometry = geometry;
        m_material = material;
    }

    Mesh::~Mesh()
    {
        SAFE_DELETE(m_geometry);
        SAFE_DELETE(m_material);
    }
}
