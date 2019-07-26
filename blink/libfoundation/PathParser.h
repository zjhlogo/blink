#pragma once
#include "BaseTypes.h"
#include <vector>

namespace blink
{
    class PathParser
    {
    public:
        PathParser();
        ~PathParser();

        void parse(const tstring& path);
        tstring getDirectory();
        tstring getFullPath();

        static tstring getFileLowerExtension(const tstring& path);

    private:
        void appendSubDir(std::vector<tstring>& dirList, const tstring& subDir);

    public:
        tstring root;
        std::vector<tstring> subDirectories;
        tstring filename;
        tstring extension;

    };
}
