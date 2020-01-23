/*!
 * \file helloworld.cpp
 *
 * \author zjhlogo
 * \date 2019/12/07
 *
 * 
 */
#pragma once

namespace noise
{
    namespace module
    {
        class ModuleBase;
    }

    class ScalarParameter
    {
    public:
        ScalarParameter(double value);
        ScalarParameter(const module::ModuleBase& src);
        ScalarParameter(const ScalarParameter& sp);

        double getValue(double x, double y, double z) const;

    private:
        const module::ModuleBase* m_pSrc{};
        double m_value{};

    };
}
