/**

    @file      Step.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      2.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <noise/module/modulebase.h>

#include <vector>

class Step : public noise::module::ModuleBase
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

    virtual int getSourceModuleCount() const override;
    virtual double getValue(double x, double y, double z) const override;

    void addStep(double step, double value);

private:
    double getStepValue(double step) const;

private:
    StepInfoList m_stepInfoList;
};
