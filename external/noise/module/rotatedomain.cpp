// rotatepoint.cpp
//
// Copyright (C) 2003, 2004 Jason Bevins
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License (COPYING.txt) for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//

#include "rotatedomain.h"

#include "../mathconsts.h"

using namespace noise::module;

RotateDomain::RotateDomain()
    : ModuleBase(1)
{
    setAngles(DEFAULT_ROTATE_X, DEFAULT_ROTATE_Y, DEFAULT_ROTATE_Z);
}

RotateDomain::RotateDomain(double xAngle, double yAngle, double zAngle)
    : ModuleBase(1)
{
    setAngles(xAngle, yAngle, zAngle);
}

void RotateDomain::setXAngle(double xAngle)
{
    setAngles(xAngle, m_yAngle, m_zAngle);
}

double RotateDomain::getXAngle() const
{
    return m_xAngle;
}

void RotateDomain::setYAngle(double yAngle)
{
    setAngles(m_xAngle, yAngle, m_zAngle);
}

double RotateDomain::getYAngle() const
{
    return m_yAngle;
}

void RotateDomain::setZAngle(double zAngle)
{
    setAngles(m_xAngle, m_yAngle, zAngle);
}

double RotateDomain::getZAngle() const
{
    return m_zAngle;
}

void RotateDomain::setAngles(double xAngle, double yAngle, double zAngle)
{
    double xCos, yCos, zCos, xSin, ySin, zSin;
    xCos = cos(xAngle * DEG_TO_RAD);
    yCos = cos(yAngle * DEG_TO_RAD);
    zCos = cos(zAngle * DEG_TO_RAD);
    xSin = sin(xAngle * DEG_TO_RAD);
    ySin = sin(yAngle * DEG_TO_RAD);
    zSin = sin(zAngle * DEG_TO_RAD);

    m_x1Matrix = ySin * xSin * zSin + yCos * zCos;
    m_y1Matrix = xCos * zSin;
    m_z1Matrix = ySin * zCos - yCos * xSin * zSin;
    m_x2Matrix = ySin * xSin * zCos - yCos * zSin;
    m_y2Matrix = xCos * zCos;
    m_z2Matrix = -yCos * xSin * zCos - ySin * zSin;
    m_x3Matrix = -ySin * xCos;
    m_y3Matrix = xSin;
    m_z3Matrix = yCos * xCos;

    m_xAngle = xAngle;
    m_yAngle = yAngle;
    m_zAngle = zAngle;
}

double RotateDomain::getValue(double x, double y, double z) const
{
    assert(m_pSourceModule[0] != NULL);

    double nx = (m_x1Matrix * x) + (m_y1Matrix * y) + (m_z1Matrix * z);
    double ny = (m_x2Matrix * x) + (m_y2Matrix * y) + (m_z2Matrix * z);
    double nz = (m_x3Matrix * x) + (m_y3Matrix * y) + (m_z3Matrix * z);
    return m_pSourceModule[0]->getValue(nx, ny, nz);
}
