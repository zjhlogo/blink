#pragma once

#include <noise/module/modulebase.h>

class Crop : public noise::module::Module
{
public:
    Crop();
    virtual ~Crop();

    virtual int GetSourceModuleCount() const override;
    virtual double GetValue(double x, double y, double z) const override;

    void SetRangeX(double minX, double maxX) { m_minX = minX; m_maxX = maxX; };
    void SetRangeY(double minY, double maxY) { m_minY = minY; m_maxY = maxY; };
    void SetRangeZ(double minZ, double maxZ) { m_minZ = minZ; m_maxZ = maxZ; };

    double GetMinX() const { return m_minX; };
    double GetMinY() const { return m_minY; };
    double GetMinZ() const { return m_minZ; };
    double GetMaxX() const { return m_maxX; };
    double GetMaxY() const { return m_maxY; };
    double GetMaxZ() const { return m_maxZ; };

private:
    double m_minX{ -999999.9 };
    double m_minY{ -999999.9 };
    double m_minZ{ -999999.9 };

    double m_maxX{ 999999.9 };
    double m_maxY{ 999999.9 };
    double m_maxZ{ 999999.9 };
};
