/*!
 * \file PathParser.h
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 * 
 */
#pragma once
#include "BaseTypes.h"
#include <vector>
#include <set>

NS_BEGIN

class PathParser
{
public:
    PathParser();
    ~PathParser();

    void parse(const tstring& path);
    tstring getDirectory();
    tstring getFullPath();

    static void filterFilesByExtension(std::set<tstring>& filesInOut, const std::set<tstring>& includeExts);
    static bool matchFileExtensions(const tstring& filePath, const std::set<tstring>& exts, bool caseSensitive = false);
    static tstring getFileLowerExtension(const tstring& path);
    static tstring combinePath(const tstring& path1, const tstring& path2);

private:
    void appendSubDir(std::vector<tstring>& dirList, const tstring& subDir);

public:
    tstring root;
    std::vector<tstring> subDirectories;
    tstring filename;
    tstring extension;

};

NS_END
