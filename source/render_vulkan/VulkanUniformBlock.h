/*********************************************************************
 * \file   VulkanUniformBlock.h
 * \brief
 *
 * \author zjhlogo
 * \date   4/25 2022
 *********************************************************************/
#pragma once

#include <core/types/UniformTypes.h>

#include <unordered_map>
#include <vector>

namespace blink
{
    class VulkanUniformBlock
    {
    private:
        struct UniformMemberInfo
        {
            tstring name;
            UniformType type;
            uint32_t offset;
        };

    public:
        void reset();
        void prepareBuffer();
        bool isReady();

        bool addUniformMember(const tstring& name, UniformType type, uint32_t offset);

        template <typename T> bool setUniformMember(const tstring& memberName, const T* value)
        {
            auto it = m_nameToIndexMap.find(memberName);
            if (it == m_nameToIndexMap.end())
            {
                return false;
            }

            return setUniformMember(it->second, value);
        }

        template <typename T> bool setUniformMember(int memberIndex, const T* value)
        {
            const auto& memberInfo = m_memberInfoList[memberIndex];
            if (getUniformType<T>() != memberInfo.type)
            {
                return false;
            }

            T* pData = reinterpret_cast<T*>(m_bufferData.data() + memberInfo.offset);
            *pData = *value;
            return true;
        }

        template <typename T> bool getUniformMember(T* valueOut, const tstring& memberName)
        {
            auto it = m_nameToIndexMap.find(memberName);
            if (it == m_nameToIndexMap.end())
            {
                return false;
            }

            return getUniformMember(valueOut, it->second);
        }

        template <typename T> bool getUniformMember(T* valueOut, int memberIndex)
        {
            const auto& memberInfo = m_memberInfoList[memberIndex];
            if (getUniformType<T>() != memberInfo.type)
            {
                return false;
            }

            *valueOut = *reinterpret_cast<T*>(m_bufferData.data() + memberInfo.offset);
            return true;
        }

        [[nodiscard]] const tstring& getMemberName(int memberIndex) const;
        [[nodiscard]] UniformType getMemberType(int memberIndex) const;
        [[nodiscard]] int getMemberCount() const { return static_cast<int>(m_memberInfoList.size()); }
        [[nodiscard]] const void* getBufferData() const { return m_bufferData.data(); }
        [[nodiscard]] uint32_t getBufferSize() const { return m_uniformStructSize; }

    private:
        static uint32_t calculateSize(UniformType type);
        [[nodiscard]] uint32_t calculateOffset(UniformType type) const;

    private:
        uint32_t m_uniformStructSize{};

        std::vector<UniformMemberInfo> m_memberInfoList;
        std::unordered_map<tstring, int> m_nameToIndexMap;

        std::vector<uint8_t> m_bufferData;
    };

    template <> bool VulkanUniformBlock::setUniformMember(int memberIndex, const glm::mat3* value);

    template <> bool VulkanUniformBlock::getUniformMember(glm::mat3* valueOut, int memberIndex);
} // namespace blink
