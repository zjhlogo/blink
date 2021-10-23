#pragma once

#include <noise/module/modulebase.h>

class Crop : public noise::module::ModuleBase
{
public:
    Crop();
    virtual ~Crop();

    virtual int getSourceModuleCount() const override;
    virtual double getValue(double x, double y, double z) const override;

    void setRangeX(double minX, double maxX) { m_minX = minX; m_maxX = maxX; };
    void setRangeY(double minY, double maxY) { m_minY = minY; m_maxY = maxY; };
    void setRangeZ(double minZ, double maxZ) { m_minZ = minZ; m_maxZ = maxZ; };

    double getMinX() const { return m_minX; };
    double getMinY() const { return m_minY; };
    double getMinZ() const { return m_minZ; };
    double getMaxX() const { return m_maxX; };
    double getMaxY() const { return m_maxY; };
    double getMaxZ() const { return m_maxZ; };

private:
    double m_minX{ -999999.9 };
    double m_minY{ -999999.9 };
    double m_minZ{ -999999.9 };

    double m_maxX{ 999999.9 };
    double m_maxY{ 999999.9 };
    double m_maxZ{ 999999.9 };
};
