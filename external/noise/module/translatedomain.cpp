// translatepoint.cpp
//
// Copyright (C) 2004 Jason Bevins
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

#include "translatedomain.h"

using namespace noise::module;

TranslateDomain::TranslateDomain(const noise::ScalarParameter& src)
    : m_source(src)
    , m_xTranslation(DEFAULT_TRANSLATE_POINT_X)
    , m_yTranslation(DEFAULT_TRANSLATE_POINT_Y)
    , m_zTranslation(DEFAULT_TRANSLATE_POINT_Z)
{
}

TranslateDomain::TranslateDomain(const noise::ScalarParameter& src, const noise::ScalarParameter& xTranslation, const noise::ScalarParameter& yTranslation, const noise::ScalarParameter& zTranslation)
    : m_source(src)
    , m_xTranslation(xTranslation)
    , m_yTranslation(yTranslation)
    , m_zTranslation(zTranslation)
{
}

void TranslateDomain::SetXTranslation(const noise::ScalarParameter& xTranslation)
{
    m_xTranslation = xTranslation;
}

const noise::ScalarParameter& TranslateDomain::GetXTranslation() const
{
    return m_xTranslation;
}

void TranslateDomain::SetYTranslation(const noise::ScalarParameter& yTranslation)
{
    m_yTranslation = yTranslation;
}

const noise::ScalarParameter& TranslateDomain::GetYTranslation() const
{
    return m_yTranslation;
}

void TranslateDomain::SetZTranslation(const noise::ScalarParameter& zTranslation)
{
    m_zTranslation = zTranslation;
}

const noise::ScalarParameter& TranslateDomain::GetZTranslation() const
{
    return m_zTranslation;
}

void TranslateDomain::SetTranslation(const noise::ScalarParameter& xTranslation, const noise::ScalarParameter& yTranslation, const noise::ScalarParameter& zTranslation)
{
    m_xTranslation = xTranslation;
    m_yTranslation = yTranslation;
    m_zTranslation = zTranslation;
}

double TranslateDomain::getValue(double x, double y, double z) const
{
    double finalX = x + m_xTranslation.getValue(x, y, z);
    double finalY = y + m_yTranslation.getValue(x, y, z);
    double finalZ = z + m_zTranslation.getValue(x, y, z);

    return m_source.getValue(finalX, finalY, finalZ);
}
