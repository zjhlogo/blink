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

NS_BEGIN

class Directory
{
public:
    static void listAllFiles(std::set<tstring>& filesOut, const tstring& rootDir, bool recursive);

private:
    static void enumlateFiles(std::set<tstring>& filesOut, const tstring& rootDir, bool recursive);
};

NS_END
