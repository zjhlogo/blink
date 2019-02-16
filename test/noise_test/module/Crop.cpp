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
    if ((x > m_minX && x < m_maxX)
        && (y > m_minY && y < m_maxY)
        && (z > m_minZ && z < m_maxZ))
    {
        return GetSourceModule(1).GetValue(x, y, z);
    }

    return GetSourceModule(0).GetValue(x, y, z);
}
