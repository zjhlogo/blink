/**

    @file      Crop.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      2.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/

#include "Crop.h"

Crop::Crop()
    : ModuleBase(2)
{
}

Crop::~Crop()
{
}

int Crop::getSourceModuleCount() const
{
    return 2;
}

double Crop::getValue(double x, double y, double z) const
{
    assert(m_pSourceModule[0] != nullptr);
    assert(m_pSourceModule[1] != nullptr);

    if ((x > m_minX && x < m_maxX) && (y > m_minY && y < m_maxY) && (z > m_minZ && z < m_maxZ))
    {
        return m_pSourceModule[1]->getValue(x, y, z);
    }

    return m_pSourceModule[0]->getValue(x, y, z);
}
