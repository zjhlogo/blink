#include "StringBuilder.h"
#include <algorithm>

namespace blink
{
    int StringBuilder::replace(tstring & strInOut, const tstring & strSearch, const tstring & strReplace)
    {
        StringList arrSubString;
        split<StringList>(arrSubString, strInOut, strSearch);

        if (arrSubString.size() > 1)
        {
            strInOut = join<StringList>(arrSubString, strReplace);
            return arrSubString.size() - 1;
        }

        return 0;
    }

    bool StringBuilder::trim(tstring & strOut, const tstring & strIn)
    {
        if (strIn.empty()) return false;

        strOut = strIn;
        strOut.erase(0, strOut.find_first_not_of(" "));
        strOut.erase(strOut.find_last_not_of(" ") + 1);

        return true;
    }

    void StringBuilder::upper(tstring & strOut, const tstring & strIn)
    {
        strOut = strIn;
        std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::toupper);
    }

    void StringBuilder::lower(tstring & strOut, const tstring & strIn)
    {
        strOut = strIn;
        std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::tolower);
    }

    bool StringBuilder::isSpace(const tstring & strIn)
    {
        for (const auto& ch : strIn)
        {
            if (ch != ' ' && ch != '\n' && ch != '\t' && ch != '\r') return false;
        }

        return true;
    }

    bool StringBuilder::hasSpace(const tstring & strIn)
    {
        for (const auto& ch : strIn)
        {
            if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r') return true;
        }
        return false;
    }

    template<> int StringBuilder::split<StringList>(StringList& arrOut, const tstring& strIn, const tstring& strSplit)
    {
        int totalSplit = 0;
        size_t startIndex = 0;
        size_t endIndex = 0;

        endIndex = strIn.find(strSplit, startIndex);
        while (endIndex != tstring::npos)
        {
            arrOut.push_back(strIn.substr(startIndex, endIndex - startIndex));
            ++totalSplit;

            startIndex = endIndex;
            startIndex += strSplit.length();
            endIndex = strIn.find(strSplit, startIndex);
        }

        if (totalSplit <= 0)
        {
            arrOut.push_back(strIn);
            ++totalSplit;
            return totalSplit;
        }

        arrOut.push_back(strIn.substr(startIndex));
        ++totalSplit;

        return totalSplit;
    }
}
