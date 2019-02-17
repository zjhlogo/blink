#include "Step.h"
#include <algorithm>

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
    assert(m_pSourceModule[0] != nullptr && "must hava a source module");
    assert(m_stepInfoList.size() > 0 && "must hava step informations");

    double value = m_pSourceModule[0]->GetValue(x, y, z);
    return GetStepValue(value);
}

void Step::AddStep(double step, double value)
{
    m_stepInfoList.push_back({ step, value });
    std::sort(m_stepInfoList.begin(), m_stepInfoList.end(), [](const StepInfo& info1, const StepInfo& info2)
    {
        return info1.step < info2.step;
    });
}

double Step::GetStepValue(double step) const
{
    for (auto& it = m_stepInfoList.rbegin(); it != m_stepInfoList.rend(); ++it)
    {
        if (step >= it->step) return it->value;
    }

    return -1.0;
}
