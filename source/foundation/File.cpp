/*!
 * \file File.cpp
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 *
 */
#include "File.h"

NS_BEGIN

File::File()
{
}

File::File(const tstring& filePath, int flag /*= AM_READ*/)
{
    open(filePath, flag);
}

File::~File()
{
    close();
}

bool File::readFileAsString(tstring& strOut, const tstring& filePath)
{
    File file;
    if (!file.open(filePath)) return false;

    auto fileSize = file.fileSize();
    strOut.resize(fileSize);
    file.read((void*)strOut.data(), fileSize);

    return true;
}

NS_END
