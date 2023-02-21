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
        static constexpr int AM_READ = 1 << 0;
        static constexpr int AM_WRITE = 1 << 1;

        enum class SeekType
        {
            Current = 0,
            Begin,
            End,
        };

    public:
        File() = default;
        explicit File(const tstring& filePath, int flag = AM_READ);
        ~File();

        File(const File& file) = delete;
        File(File&& file) = delete;
        File& operator=(const File& file) = delete;
        File& operator=(File&& file) = delete;

        bool open(const tstring& filePath, int flag = AM_READ);
        void close();

        int64_t seek(int64_t offset, SeekType seekType = SeekType::Current);
        int64_t tell();

        int64_t read(void* dataOut, int64_t size);
        int64_t read(std::vector<uint8_t>& dataOut, int64_t size);

        int64_t write(const void* data, int64_t size);
        int64_t write(const std::vector<uint8_t>& data);

        int64_t fileSize() const { return m_fileSize; }

        static bool readFileIntoString(tstring& strOut, const tstring& filePath);
        static bool readFileIntoBuffer(std::vector<uint8_t>& dataOut, const tstring& filePath);

        static bool initializeRoot(const tstring& rootDir);

    private:
        void* m_fileHandler{};
        int64_t m_fileSize{};
        int m_mode{AM_READ};

        static tstring m_rootDir;

    };
} // namespace blink
