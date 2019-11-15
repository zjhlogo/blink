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

NS_END