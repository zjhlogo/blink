// gradient.cpp
//
// Copyright (C) 2019, 2020 zjhlogo
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
// The developer's email is zjhlogo@gmzigail.com (for great email, take
// off every 'zig'.)
//

#include "gradient.h"

using namespace noise::module;

Gradient::Gradient(double x1, double x2, double y1, double y2, double z1 /*= 0.0*/, double z2 /*= 0.0*/)
    : ModuleBase(0)
{
    setGradient(x1, x2, y1, y2, z1, z2);
}

void Gradient::setGradient(double x1, double x2, double y1, double y2, double z1 /*= 0.0*/, double z2 /*= 0.0*/)
{
    m_gx1 = x1;
    m_gy1 = y1;
    m_gz1 = z1;

    m_x = x2 - x1;
    m_y = y2 - y1;
    m_z = z2 - z1;

    m_vlen = (m_x * m_x + m_y * m_y + m_z * m_z);
}

double Gradient::getValue(double x, double y, double z) const
{
    double dx = x - m_gx1;
    double dy = y - m_gy1;
    double dz = z - m_gz1;
    double dp = dx * m_x + dy * m_y + dz * m_z;
    dp /= m_vlen;

    return (dp - 0.5) * 2.0;  // normalize it back to [-1, 1]
}
