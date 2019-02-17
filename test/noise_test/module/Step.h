#pragma once

#include <noise/module/modulebase.h>
#include <vector>

class Step : public noise::module::Module
{
public:
    class StepInfo
    {
    public:
        double step;
        double value;
    };

    typedef std::vector<StepInfo> StepInfoList;

public:
    Step();
    virtual ~Step();

    virtual int GetSourceModuleCount() const override;
    virtual double GetValue(double x, double y, double z) const override;

    void AddStep(double step, double value);

private:
    double GetStepValue(double step) const;

private:
    StepInfoList m_stepInfoList;

};
