/*!
 * \file Directory.cpp
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 *
 */
#include "Directory.h"

namespace blink
{
    void Directory::listAllFiles(std::set<tstring>& filesOut, const tstring& rootDir, bool recursive)
    {
        //
        enumlateFiles(filesOut, rootDir, recursive);
    }

} // namespace blink
