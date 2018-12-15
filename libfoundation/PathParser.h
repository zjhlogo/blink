#pragma once
#include "BaseType.h"

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

    private:
        void appendSubDir(StringList& dirList, const tstring& subDir);

    public:
        tstring root;
        StringList subDirectories;
        tstring filename;
        tstring extension;

    };
}
