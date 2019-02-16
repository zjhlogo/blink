#pragma once

#include <noise/module/modulebase.h>

class Step : public noise::module::Module
{
public:
    Step();
    virtual ~Step();

    virtual int GetSourceModuleCount() const override;
    virtual double GetValue(double x, double y, double z) const override;

    void SetStep(double step) { m_step = step; };
    double GetStep() const { return m_step; };

    void SetLowerValue(double value) { m_lowerValue = value; };
    double GetLowerValue() const { return m_lowerValue; };

    void SetUpperValue(double value) { m_upperValue = value; };
    double GetUpperValue() const { return m_upperValue; };

private:
    double m_step{};
    double m_lowerValue{ -1.0 };
    double m_upperValue{ 1.0 };

};
