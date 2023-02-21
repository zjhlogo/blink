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
#include "../../PathParser.h"

namespace blink
{
    size_t getFileSize(const tstring& absPath)
    {
        struct stat sb
        {
        };
        if (stat(absPath.c_str(), &sb) == 0)
        {
            return sb.st_size;
        }

        return 0;
    }

    bool File::open(const tstring& filePath, int flag /*= AM_READ*/)
    {
        tstring strMode;
        if (flag & AM_READ)
        {
            strMode += "r";
        }
        if (flag & AM_WRITE)
        {
            strMode += "w";
        }
        strMode += "b";

        auto absPath = PathParser::combinePath(m_rootDir, filePath);

        FILE* pFile = _fsopen(absPath.c_str(), strMode.c_str(), _SH_DENYWR);
        if (!pFile)
        {
            LOGE("open file failed {0}, errno: {1}.", absPath, errno);
            return false;
        }

        m_mode = flag;
        m_fileHandler = pFile;
        m_fileSize = static_cast<int64_t>(getFileSize(absPath));

        return true;
    }

    void File::close()
    {
        if (m_fileHandler)
        {
            fclose(static_cast<FILE*>(m_fileHandler));
            m_fileHandler = nullptr;
        }
    }

    int64_t File::seek(int64_t offset, SeekType seekType /* = SeekType::Current */)
    {
        if (!m_fileHandler)
        {
            return 0;
        }

        int seekFrom = SEEK_CUR;
        if (seekType == SeekType::Begin)
        {
            seekFrom = SEEK_SET;
        }
        else if (seekType == SeekType::End)
        {
            seekFrom = SEEK_END;
        }

        return fseek(static_cast<FILE*>(m_fileHandler), static_cast<long>(offset), seekFrom);
    }

    int64_t File::tell()
    {
        if (!m_fileHandler)
        {
            return 0;
        }

        return ftell(static_cast<FILE*>(m_fileHandler));
    }

    int64_t File::read(void* dataOut, int64_t size)
    {
        if (!m_fileHandler)
        {
            return 0;
        }
        if ((m_mode & AM_READ) == 0)
        {
            return 0;
        }
        if (size <= 0)
        {
            return 0;
        }

        return fread(dataOut, 1, static_cast<size_t>(size), static_cast<FILE*>(m_fileHandler));
    }

    int64_t File::read(std::vector<uint8_t>& dataOut, int64_t size)
    {
        if (!m_fileHandler)
        {
            return 0;
        }
        if ((m_mode & AM_READ) == 0)
        {
            return 0;
        }
        if (size <= 0)
        {
            return 0;
        }

        dataOut.resize(static_cast<size_t>(size));
        return fread(dataOut.data(), 1, static_cast<size_t>(size), static_cast<FILE*>(m_fileHandler));
    }

    int64_t File::write(const void* data, int64_t size)
    {
        if (!m_fileHandler)
        {
            return 0;
        }
        if ((m_mode & AM_WRITE) == 0)
        {
            return 0;
        }
        if (size <= 0)
        {
            return 0;
        }

        return fwrite(data, 1, static_cast<size_t>(size), static_cast<FILE*>(m_fileHandler));
    }

    int64_t File::write(const std::vector<uint8_t>& data)
    {
        if (!m_fileHandler)
        {
            return 0;
        }
        if ((m_mode & AM_WRITE) == 0)
        {
            return 0;
        }

        return fwrite(data.data(), 1, data.size(), static_cast<FILE*>(m_fileHandler));
    }
} // namespace blink
