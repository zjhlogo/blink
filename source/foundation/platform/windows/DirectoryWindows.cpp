/*!
 * \file DirectoryWindows.cpp
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 *
 */
#include "../../Directory.h"
#include "../../PathParser.h"
#include "dirent_windows.h"

namespace blink
{
    static const tstring CURRENT_DIR = ".";
    static const tstring PARENT_DIR = "..";

    void Directory::enumerateFiles(std::set<tstring>& filesOut, const tstring& rootDir, bool recursive)
    {
        DIR* pDir = opendir(rootDir.c_str());
        if (pDir)
        {
            /* print all the files and directories within directory */
            dirent* pEnt = nullptr;
            while ((pEnt = readdir(pDir)) != nullptr)
            {
                if (pEnt->d_type == DT_DIR)
                {
                    if (CURRENT_DIR != pEnt->d_name && PARENT_DIR != pEnt->d_name)
                    {
                        tstring currDir = PathParser::combinePath(rootDir, pEnt->d_name);
                        if (recursive)
                        {
                            enumerateFiles(filesOut, currDir, recursive);
                        }
                        else
                        {
                            filesOut.insert(currDir);
                        }
                    }
                }
                else if (pEnt->d_type == DT_REG)
                {
                    tstring currFilePath = PathParser::combinePath(rootDir, pEnt->d_name);
                    filesOut.insert(currFilePath);
                }
            }
            closedir(pDir);
        }
    }

} // namespace blink
