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

        static tstring getFileLowerExtension(const tstring& path);

    private:
        void appendSubDir(StringList& dirList, const tstring& subDir);

    public:
        tstring root;
        StringList subDirectories;
        tstring filename;
        tstring extension;

    };
}
