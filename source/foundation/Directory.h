/*!
 * \file Directory.h
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 *
 */
#pragma once
#include "BaseTypes.h"

#include <set>

namespace blink
{
    class Directory
    {
    public:
        static void listAllFiles(std::set<tstring>& filesOut, const tstring& rootDir, bool recursive);

    private:
        static void enumlateFiles(std::set<tstring>& filesOut, const tstring& rootDir, bool recursive);
    };

} // namespace blink
