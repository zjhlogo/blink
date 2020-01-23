/*!
 * \file helloworld.cpp
 *
 * \author zjhlogo
 * \date 2019/12/07
 *
 * 
 */
#include "scalarparameter.h"
#include "module/modulebase.h"

using namespace noise;

ScalarParameter::ScalarParameter(double value)
    : m_value(value)
{
}

ScalarParameter::ScalarParameter(const module::ModuleBase& src)
    : m_pSrc(&src)
{
}

ScalarParameter::ScalarParameter(const ScalarParameter& sp)
    : m_value(sp.m_value)
    , m_pSrc(sp.m_pSrc)
{
}

double ScalarParameter::getValue(double x, double y, double z) const
{
    if (m_pSrc) return m_pSrc->getValue(x, y, z);
    return m_value;
}
