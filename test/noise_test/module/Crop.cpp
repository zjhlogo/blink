#include "Crop.h"

Crop::Crop()
    :Module(2)
{
}

Crop::~Crop()
{
}

int Crop::GetSourceModuleCount() const
{
    return 2;
}

double Crop::GetValue(double x, double y, double z) const
{
    assert(m_pSourceModule[0] != nullptr);
    assert(m_pSourceModule[1] != nullptr);

    if ((x > m_minX && x < m_maxX)
        && (y > m_minY && y < m_maxY)
        && (z > m_minZ && z < m_maxZ))
    {
        return m_pSourceModule[1]->GetValue(x, y, z);
    }

    return m_pSourceModule[0]->GetValue(x, y, z);
}
