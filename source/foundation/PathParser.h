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

#include <set>
#include <vector>

namespace blink
{
    class PathParser
    {
    public:
        bool isValid();
        void reset();

        void parse(const tstring& path);

        /**
         * get the full path. contain the root + sub directories + file name, and formatted the path separator to '/'
         *
         * \return
         */
        tstring getFullPath() const;

        /**
         * the directory always end with '/'
         *
         * \return
         */
        tstring getDirectory() const;

        tstring replaceFileName(const tstring& fileName) const;

        static void filterFilesByExtension(std::set<tstring>& filesInOut, const std::set<tstring>& includeExts);
        static bool matchFileExtensions(const tstring& filePath, const std::set<tstring>& exts, bool caseSensitive = false);
        static tstring getFileLowerExtension(const tstring& path);
        static tstring combinePath(const tstring& path1, const tstring& path2);
        static tstring getAbsolutePath(const tstring& path, const tstring& referenceDir);
        static tstring formatPath(const tstring& path);

    private:
        void appendSubDir(std::vector<tstring>& dirList, const tstring& subDir);

    public:
        tstring root;
        std::vector<tstring> subDirectories;
        tstring filename;
        tstring extension;
    };
} // namespace blink
