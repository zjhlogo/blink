/*!
 * \file StringBuilder.h
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 *
 */
#pragma once
#include "BaseTypes.h"

#include <fmt/format.h>
#include <set>
#include <sstream>
#include <vector>

NS_BEGIN

class StringBuilder
{
public:
    static int replace(tstring& strInOut, const tstring& strSearch, const tstring& strReplace);
    static bool trim(tstring& strOut, const tstring& strIn);

    static tstring upper(const tstring& strIn);
    static tstring lower(const tstring& strIn);

    static bool isSpace(const tstring& strIn);
    static bool hasSpace(const tstring& strIn);

    template <typename CONTAINER> static int split(CONTAINER& arrOut, const tstring& strIn, const tstring& strSplit)
    {
        std::vector<tstring> arrString;
        if (split(arrString, strIn, strSplit) <= 0) return 0;

        for (const auto& strValue : arrString)
        {
            std::stringstream ss(strValue);
            typename CONTAINER::value_type value;
            ss >> value;
            if (!ss)
            {
                arrOut.clear();
                return 0;
            }
            arrOut.push_back(value);
        }

        return arrOut.size();
    }

    template <typename T> static int split(std::set<T>& arrOut, const tstring& strIn, const tstring& strSplit)
    {
        std::vector<tstring> arrString;
        if (split(arrString, strIn, strSplit) <= 0) return 0;

        for (const auto& strValue : arrString)
        {
            std::stringstream ss(strValue);
            T value;
            ss >> value;
            if (!ss)
            {
                arrOut.clear();
                return 0;
            }

            arrOut.insert(value);
        }

        return arrOut.size();
    }

    template <typename CONTAINER> static tstring join(const CONTAINER& arrIn, const tstring& strConnect, bool reverse = false)
    {
        tstring strOut;

        int nLength = int(arrIn.size());
        if (nLength <= 0) return strOut;

        if (reverse)
        {
            auto it = arrIn.rbegin();
            strOut += fmt::format("{0}", *it++);

            for (; it != arrIn.rend(); ++it)
            {
                strOut += strConnect;
                strOut += fmt::format("{0}", *it);
            }
        }
        else
        {
            auto it = arrIn.begin();
            strOut += fmt::format("{0}", *it++);

            for (; it != arrIn.end(); ++it)
            {
                strOut += strConnect;
                strOut += fmt::format("{0}", *it);
            }
        }

        return strOut;
    }
};

template <>
int StringBuilder::split<std::vector<tstring>>(std::vector<tstring>& arrOut, const tstring& strIn, const tstring& strSplit);

NS_END
