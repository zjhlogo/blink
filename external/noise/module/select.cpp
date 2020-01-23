// select.cpp
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

#include "select.h"
#include "../scalarparameter.h"
#include "../interp.h"

using namespace noise::module;

Select::Select(const noise::ScalarParameter& low, const noise::ScalarParameter& high, const noise::ScalarParameter& control)
    : m_edgeFalloff(DEFAULT_SELECT_EDGE_FALLOFF)
    , m_threshold(DEFAULT_SELECT_THRESHOLD)
    , m_low(low)
    , m_high(high)
    , m_control(control)
{
}

Select::Select(const noise::ScalarParameter& low, const noise::ScalarParameter& high, const noise::ScalarParameter& control, const noise::ScalarParameter& threshold, const noise::ScalarParameter& edgeFalloff)
    : m_edgeFalloff(edgeFalloff)
    , m_threshold(threshold)
    , m_low(low)
    , m_high(high)
    , m_control(control)
{
}

void Select::setLowModule(const noise::ScalarParameter& low)
{
    m_low = low;
}

const noise::ScalarParameter& Select::getLowModule() const
{
    return m_low;
}

void Select::setHighModule(const noise::ScalarParameter& high)
{
    m_high = high;
}

const noise::ScalarParameter& Select::getHighModule() const
{
    return m_high;
}

void Select::setControlModule(const noise::ScalarParameter& control)
{
    m_control = control;
}

const noise::ScalarParameter& Select::getControlModule() const
{
    return m_control;
}

void Select::setThreshold(const noise::ScalarParameter& threshold)
{
    m_threshold = threshold;
}

const noise::ScalarParameter& Select::getThreshold() const
{
    return m_threshold;
}

void Select::setEdgeFalloff(const noise::ScalarParameter& edgeFalloff)
{
    m_edgeFalloff = edgeFalloff;
}

const noise::ScalarParameter& Select::getEdgeFalloff() const
{
    return m_edgeFalloff;
}

double Select::getValue(double x, double y, double z) const
{
    double controlValue = m_control.getValue(x, y, z);
    double fallOffValue = m_edgeFalloff.getValue(x, y, z);
    double threshold = m_threshold.getValue(x, y, z);

    if (fallOffValue > 0.0)
    {
        if (controlValue < (threshold - fallOffValue))
        {
            // The output value from the control module is below the selector
            // threshold; return the output value from the first source module.
            return m_low.getValue(x, y, z);
        }
        else if (controlValue > (threshold + fallOffValue))
        {
            return m_high.getValue(x, y, z);
        }
        else
        {
            // The output value from the control module is near the lower end of the
            // selector threshold and within the smooth curve. Interpolate between
            // the output values from the first and second source modules.
            double lowerCurve = (threshold - fallOffValue);
            double upperCurve = (threshold + fallOffValue);
            double alpha = SCurve3((controlValue - lowerCurve) / (upperCurve - lowerCurve));
            return LinearInterp(m_low.getValue(x, y, z), m_high.getValue(x, y, z), alpha);
        }
    }
    else
    {
        if (controlValue < threshold)
        {
            return m_low.getValue(x, y, z);
        }
        else
        {
            return m_high.getValue(x, y, z);
        }
    }
}
