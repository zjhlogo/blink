// scalepoint.cpp
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

#include "scaledomain.h"

using namespace noise::module;

ScaleDomain::ScaleDomain(const noise::ScalarParameter& src)
    : m_source(src)
    , m_xScale(DEFAULT_SCALE_POINT_X)
    , m_yScale(DEFAULT_SCALE_POINT_Y)
    , m_zScale(DEFAULT_SCALE_POINT_Z)
{
}

ScaleDomain::ScaleDomain(const noise::ScalarParameter& src, const noise::ScalarParameter& scale)
    : m_source(src)
    , m_xScale(scale)
    , m_yScale(scale)
    , m_zScale(scale)
{
}

ScaleDomain::ScaleDomain(const noise::ScalarParameter& src, const noise::ScalarParameter& xScale, const noise::ScalarParameter& yScale, const noise::ScalarParameter& zScale)
    : m_source(src)
    , m_xScale(xScale)
    , m_yScale(yScale)
    , m_zScale(zScale)
{
}

void ScaleDomain::SetXScale(const noise::ScalarParameter& xScale)
{
    m_xScale = xScale;
}

const noise::ScalarParameter& ScaleDomain::GetXScale() const
{
    return m_xScale;
}

void ScaleDomain::SetYScale(const noise::ScalarParameter& yScale)
{
    m_yScale = yScale;
}

const noise::ScalarParameter& ScaleDomain::GetYScale() const
{
    return m_yScale;
}

void ScaleDomain::SetZScale(const noise::ScalarParameter& zScale)
{
    m_zScale = zScale;
}

const noise::ScalarParameter& ScaleDomain::GetZScale() const
{
    return m_zScale;
}

void ScaleDomain::SetScale(const noise::ScalarParameter& scale)
{
    m_xScale = scale;
    m_yScale = scale;
    m_zScale = scale;
}

void ScaleDomain::SetScale(const noise::ScalarParameter& xScale, const noise::ScalarParameter& yScale, const noise::ScalarParameter& zScale)
{
    m_xScale = xScale;
    m_yScale = yScale;
    m_zScale = zScale;
}

double ScaleDomain::getValue(double x, double y, double z) const
{
    double finalX = x * m_xScale.getValue(x, y, z);
    double finalY = y * m_yScale.getValue(x, y, z);
    double finalZ = z * m_zScale.getValue(x, y, z);

    return m_source.getValue(finalX, finalY, finalZ);
}
