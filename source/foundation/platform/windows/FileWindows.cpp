/*!
 * \file FileWindows.cpp
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 * 
 */
#include "../../File.h"
#include "../../Log.h"

NS_BEGIN

size_t getFileSize(const tstring& filePath)
{
    struct stat sb;
    if (stat(filePath.c_str(), &sb) == 0)
    {
        return sb.st_size;
    }

    return 0;
}

bool File::open(const tstring& filePath, int flag /*= AM_READ*/)
{
    tstring strMode;
    if (flag & AM_READ) strMode += "r";
    if (flag & AM_WRITE) strMode += "w";
    strMode += "b";

    FILE* pFile = _fsopen(filePath.c_str(), strMode.c_str(), _SH_DENYWR);
    if (!pFile)
    {
        LOGE("open file failed {0}, errno: {1}.", filePath, errno);
        return false;
    }

    m_mode = flag;
    m_fileHandler = pFile;
    m_fileSize = getFileSize(filePath);

    return true;
}

void File::close()
{
    if (m_fileHandler)
    {
        fclose((FILE*)m_fileHandler);
        m_fileHandler = nullptr;
    }
}

int64 File::seek(int64 offset, SeekType seekType /* = SeekType::Current */)
{
    if (!m_fileHandler) return 0;

    int seekFrom = SEEK_CUR;
    if (seekType == SeekType::Begin) seekFrom = SEEK_SET;
    else if (seekType == SeekType::End) seekFrom = SEEK_END;

    return fseek((FILE*)m_fileHandler, static_cast<long>(offset), seekFrom);
}

int64 File::tell()
{
    if (!m_fileHandler) return 0;

    return ftell((FILE*)m_fileHandler);
}

int64 File::read(void* dataOut, int64 size)
{
    if (!m_fileHandler) return 0;
    if ((m_mode & AM_READ) == 0) return 0;
    if (size <= 0) return 0;

    return fread(dataOut, 1, static_cast<size_t>(size), (FILE*)m_fileHandler);
}

int64 File::read(std::vector<uint8>& dataOut, int64 size)
{
    if (!m_fileHandler) return 0;
    if ((m_mode & AM_READ) == 0) return 0;
    if (size <= 0) return 0;

    dataOut.resize(static_cast<size_t>(size));
    return fread(dataOut.data(), 1, static_cast<size_t>(size), (FILE*)m_fileHandler);
}

int64 File::write(const void* data, int64 size)
{
    if (!m_fileHandler) return 0;
    if ((m_mode & AM_WRITE) == 0) return 0;
    if (size <= 0) return 0;

    return fwrite(data, 1, static_cast<size_t>(size), (FILE*)m_fileHandler);
}

int64 File::write(const std::vector<uint8>& data)
{
    if (!m_fileHandler) return 0;
    if ((m_mode & AM_WRITE) == 0) return 0;

    return fwrite(data.data(), 1, data.size(), (FILE*)m_fileHandler);
}

NS_END
