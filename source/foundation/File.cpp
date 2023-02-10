/*!
 * \file File.cpp
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 *
 */
#include "File.h"

namespace blink
{
    File::File(const tstring& filePath, int flag /*= AM_READ*/)
    {
        //
        open(filePath, flag);
    }

    File::~File()
    {
        //
        close();
    }

    bool File::readFileIntoString(tstring& strOut, const tstring& filePath)
    {
        File file;
        if (!file.open(filePath)) return false;

        auto fileSize = file.fileSize();
        strOut.resize(fileSize);
        file.read(&strOut[0], fileSize);

        return true;
    }

    bool File::readFileIntoBuffer(std::vector<uint8_t>& dataOut, const tstring& filePath)
    {
        File file;
        if (!file.open(filePath)) return false;

        auto fileSize = file.fileSize();
        dataOut.resize(fileSize);
        file.read((void*)dataOut.data(), fileSize);

        return true;
    }
} // namespace blink
