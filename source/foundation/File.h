/*!
 * \file File.h
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 *
 */
#pragma once
#include "BaseTypes.h"

#include <vector>

namespace blink
{
    class File
    {
    public:
        // access mode
        static const int AM_READ = 1 << 0;
        static const int AM_WRITE = 1 << 1;

        enum class SeekType
        {
            Current = 0,
            Begin,
            End,
        };

    public:
        File();
        File(const tstring& filePath, int flag = AM_READ);
        ~File();

        bool open(const tstring& filePath, int flag = AM_READ);
        void close();

        int64 seek(int64 offset, SeekType seekType = SeekType::Current);
        int64 tell();

        int64 read(void* dataOut, int64 size);
        int64 read(std::vector<uint8>& dataOut, int64 size);

        int64 write(const void* data, int64 size);
        int64 write(const std::vector<uint8>& data);

        int64 fileSize() const { return m_fileSize; };

        static bool readFileIntoString(tstring& strOut, const tstring& filePath);
        static bool readFileIntoBuffer(std::vector<uint8>& dataOut, const tstring& filePath);

    private:
        void* m_fileHandler{};
        int64 m_fileSize{};
        int m_mode{AM_READ};
    };

} // namespace blink
