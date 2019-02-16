#include "Step.h"

Step::Step()
    :Module(1)
{
}

Step::~Step()
{
}

int Step::GetSourceModuleCount() const
{
    return 1;
}

double Step::GetValue(double x, double y, double z) const
{
    double value = GetSourceModule(0).GetValue(x, y, z);
    if (value <= m_step) return m_lowerValue;
    return m_upperValue;
}
