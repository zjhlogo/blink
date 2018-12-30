#include "Mesh.h"

namespace blink
{
    Mesh::Mesh(Geometry * geometry, Material * material)
    {
        SAFE_OBTAIN(m_geometry, geometry);
        SAFE_OBTAIN(m_material, material);
    }

    Mesh::~Mesh()
    {
        SAFE_RELEASE(m_geometry);
        SAFE_RELEASE(m_material);
    }
}
