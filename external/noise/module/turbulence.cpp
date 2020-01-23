// turbulence.cpp
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

#include "turbulence.h"

using namespace noise::module;

Turbulence::Turbulence(const noise::ScalarParameter& src)
    : m_source(src)
    , m_power(DEFAULT_TURBULENCE_POWER)
{
    setSeed(DEFAULT_TURBULENCE_SEED);
    setFrequency(DEFAULT_TURBULENCE_FREQUENCY);
    setRoughness(DEFAULT_TURBULENCE_ROUGHNESS);
}

Turbulence::Turbulence(const noise::ScalarParameter& src, double frequency, double power, int roughness)
    : m_source(src)
    , m_power(power)
{
    setSeed(DEFAULT_TURBULENCE_SEED);
    setFrequency(frequency);
    setRoughness(roughness);
}

void Turbulence::setFrequency(double frequency)
{
    // Set the frequency of each Perlin-noise module.
    m_xDistortModule.setFrequency(frequency);
    m_yDistortModule.setFrequency(frequency);
    m_zDistortModule.setFrequency(frequency);
}

double Turbulence::getFrequency() const
{
    // Since each noise::module::Perlin noise module has the same frequency, it
    // does not matter which module we use to retrieve the frequency.
    return m_xDistortModule.getFrequency();
}

void Turbulence::setPower(double power)
{
    m_power = power;
}

double Turbulence::getPower() const
{
    return m_power;
}

void Turbulence::setRoughness(int roughness)
{
    // Set the octave count for each Perlin-noise module.
    m_xDistortModule.setOctaveCount(roughness);
    m_yDistortModule.setOctaveCount(roughness);
    m_zDistortModule.setOctaveCount(roughness);
}

int Turbulence::getRoughness() const
{
    return m_xDistortModule.getOctaveCount();
}

void Turbulence::setSeed(int seed)
{
    // Set the seed of each noise::module::Perlin noise modules.  To prevent any
    // sort of weird artifacting, use a slightly different seed for each noise
    // module.
    m_xDistortModule.setSeed(seed);
    m_yDistortModule.setSeed(seed + 1);
    m_zDistortModule.setSeed(seed + 2);
}

int Turbulence::getSeed() const
{
    return m_xDistortModule.getSeed();
}

double Turbulence::getValue(double x, double y, double z) const
{
    // Get the values from the three noise::module::Perlin noise modules and
    // add each value to each coordinate of the input value.  There are also
    // some offsets added to the coordinates of the input values.  This prevents
    // the distortion modules from returning zero if the (x, y, z) coordinates,
    // when multiplied by the frequency, are near an integer boundary.  This is
    // due to a property of gradient coherent noise, which returns zero at
    // integer boundaries.
    double x0, y0, z0;
    double x1, y1, z1;
    double x2, y2, z2;
    x0 = x + (12414.0 / 65536.0);
    y0 = y + (65124.0 / 65536.0);
    z0 = z + (31337.0 / 65536.0);
    x1 = x + (26519.0 / 65536.0);
    y1 = y + (18128.0 / 65536.0);
    z1 = z + (60493.0 / 65536.0);
    x2 = x + (53820.0 / 65536.0);
    y2 = y + (11213.0 / 65536.0);
    z2 = z + (44845.0 / 65536.0);
    double xDistort = x + (m_xDistortModule.getValue(x0, y0, z0) * m_power);
    double yDistort = y + (m_yDistortModule.getValue(x1, y1, z1) * m_power);
    double zDistort = z + (m_zDistortModule.getValue(x2, y2, z2) * m_power);

    // Retrieve the output value at the offsetted input value instead of the
    // original input value.
    return m_source.getValue(xDistort, yDistort, zDistort);
}
