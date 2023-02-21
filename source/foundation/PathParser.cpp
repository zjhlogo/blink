/*!
 * \file PathParser.cpp
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 *
 */
#include "PathParser.h"
#include "StringBuilder.h"

#include <algorithm>

namespace blink
{
    bool PathParser::isValid()
    {
        return !root.empty() || !subDirectories.empty() || !filename.empty() || !extension.empty();
    }

    void PathParser::reset()
    {
        // clear all informations
        root.clear();
        subDirectories.clear();
        filename.clear();
        extension.clear();
    }

    void PathParser::parse(const tstring& path)
    {
        reset();

        int beginPos = 0;
        // check if the path start with '/' or '\\', if so mark root flag
        if (path.length() > 0 && (path[0] == '/' || path[0] == '\\'))
        {
            root = "/";
        }
        // check if is the window style path
        else if (path.length() > 1 && path[1] == ':')
        {
            char lowerLetter = path[0] | 0x20;
            if (lowerLetter >= 'a' && lowerLetter <= 'z')
            {
                root = fmt::format("{}:/", lowerLetter);
                beginPos = 2;
            }
        }

        // find out the name parts and directory parts
        tstring dirPart;
        tstring namePart;
        int pos1 = static_cast<int>(path.rfind('/'));
        int pos2 = static_cast<int>(path.rfind('\\'));
        auto pos = static_cast<size_t>(std::max(pos1, pos2));

        if (pos != tstring::npos)
        {
            dirPart = path.substr(beginPos, pos - beginPos + 1);
            namePart = path.substr(pos + 1);
        }
        else
        {
            namePart = path;
        }

        // parse the file name and/or extension
        pos = namePart.rfind('.');
        if (pos != tstring::npos)
        {
            filename = namePart.substr(0, pos);
            extension = namePart.substr(pos + 1);
        }
        else
        {
            filename = namePart;
        }

        // parse the sub-directories from the full directory parts
        tstring subDir;
        for (const auto& ch : dirPart)
        {
            if (ch == '/' || ch == '\\')
            {
                appendSubDir(subDirectories, subDir);
                subDir.clear();
                continue;
            }

            subDir.push_back(ch);
        }

        appendSubDir(subDirectories, subDir);
        subDir.clear();
    }

    tstring PathParser::getDirectory() const
    {
        tstring directory = root;
        for (const auto& it : subDirectories)
        {
            directory.append(it);
            directory.push_back('/');
        }

        return directory;
    }

    tstring PathParser::replaceFileName(const tstring& fileName) const
    {
        tstring directory = root;
        for (const auto& it : subDirectories)
        {
            directory.append(it);
            directory.push_back('/');
        }

        directory.append(fileName);
        return directory;
    }

    tstring PathParser::getFullPath() const
    {
        tstring fullPath = getDirectory();

        fullPath.append(filename);
        if (!extension.empty())
        {
            fullPath.push_back('.');
            fullPath.append(extension);
        }

        return fullPath;
    }

    void PathParser::filterFilesByExtension(std::set<tstring>& filesInOut, const std::set<tstring>& includeExts)
    {
        std::set<tstring> oldFiles = filesInOut;
        filesInOut.clear();
        for (const auto& filePath : oldFiles)
        {
            if (matchFileExtensions(filePath, includeExts))
            {
                filesInOut.insert(filePath);
            }
        }
    }

    bool PathParser::matchFileExtensions(const tstring& filePath, const std::set<tstring>& exts, bool caseSensitive /*= false*/)
    {
        if (exts.size() <= 0)
        {
            return true;
        }

        PathParser pp;
        pp.parse(filePath);

        if (!caseSensitive)
        {
            tstring lowerFileExt = pp.extension;
            std::transform(lowerFileExt.begin(), lowerFileExt.end(), lowerFileExt.begin(), ::tolower);

            std::set<tstring> lowerExts;
            for (const auto& ext : exts)
            {
                tstring lowerExt = ext;
                std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(), ::tolower);
                lowerExts.insert(lowerExt);
            }

            if (lowerExts.find(lowerFileExt) != lowerExts.end())
            {
                return true;
            }
        }
        else
        {
            if (exts.find(pp.extension) != exts.end())
            {
                return true;
            }
        }

        return false;
    }

    tstring PathParser::getFileLowerExtension(const tstring& path)
    {
        PathParser pp;
        pp.parse(path);
        return StringBuilder::lower(pp.extension);
    }

    tstring PathParser::combinePath(const tstring& path1, const tstring& path2)
    {
        if (path1.length() > 0)
        {
            const char lastChar = path1[path1.length() - 1];
            if (lastChar == '/' || lastChar == '\\')
            {
                if (path2.length() > 0 && (path2[0] == '/' || path2[0] == '\\'))
                {
                    return (path1 + path2.substr(1));
                }

                return (path1 + path2);
            }
            else
            {
                if (path2.length() > 0 && (path2[0] == '/' || path2[0] == '\\'))
                {
                    return (path1 + path2);
                }

                return (path1 + '/') + path2;
            }
        }
        else
        {
            return path2;
        }
    }

    tstring PathParser::getAbsolutePath(const tstring& path, const tstring& referenceDir)
    {
        PathParser parser;
        parser.parse(path);

        if (!parser.root.empty())
        {
            return parser.getFullPath();
        }

        PathParser parser2;
        parser2.parse(referenceDir);
        return PathParser::combinePath(parser2.getDirectory(), parser.getFullPath());
    }

    tstring PathParser::formatPath(const tstring& path)
    {
        PathParser parser;
        parser.parse(path);
        return parser.getFullPath();
    }

    void PathParser::appendSubDir(std::vector<tstring>& dirList, const tstring& subDir)
    {
        if (subDir.empty() || subDir == ".")
        {
            return;
        }

        if (subDir == "..")
        {
            dirList.pop_back();
        }
        else
        {
            dirList.push_back(subDir);
        }
    }

} // namespace blink
