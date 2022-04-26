
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
            uint32 offset;
        };

    public:
        VulkanUniformBlock() = default;
        ~VulkanUniformBlock() = default;

        void reset();
        void prepareBuffer();

        bool addUniformMember(const tstring& name, UniformType type, uint32 offset);

        template <typename T> bool setUniformMember(const tstring& memberName, const T& value)
        {
            auto it = m_nameToIndexMap.find(memberName);
            if (it == m_nameToIndexMap.end()) return false;

            return setUniformMember(it->second, value);
        }

        template <typename T> bool setUniformMember(int memberIndex, const T& value)
        {
            const auto& memberInfo = m_memberInfoList[memberIndex];
            if (getUniformType<T>() != memberInfo.type) return false;

            T* pData = (T*)(m_bufferData.data() + memberInfo.offset);
            *pData = value;
            return true;
        }

        template <> bool setUniformMember(int memberIndex, const glm::mat3& value)
        {
            const auto& memberInfo = m_memberInfoList[memberIndex];
            if (UniformType::Mat3 != memberInfo.type) return false;

            glm::vec3* pData1 = (glm::vec3*)(m_bufferData.data() + memberInfo.offset);
            glm::vec3* pData2 = (glm::vec3*)(m_bufferData.data() + memberInfo.offset + 16);
            glm::vec3* pData3 = (glm::vec3*)(m_bufferData.data() + memberInfo.offset + 32);

            *pData1 = value[0];
            *pData2 = value[1];
            *pData3 = value[2];
            return true;
        }

        const tstring& getMemberName(int memberIndex) const;
        UniformType getMemberType(int memberIndex) const;
        int getMemberCount() const { return static_cast<int>(m_memberInfoList.size()); };
        const void* getBufferData() const { return m_bufferData.data(); };
        uint32 getBufferSize() const { return m_uniformStructSize; };

    private:
        uint32 calculateSize(UniformType type) const;
        uint32 calculateOffset(UniformType type) const;

    private:
        uint32 m_uniformStructSize{};

        std::vector<UniformMemberInfo> m_memberInfoList;
        std::unordered_map<tstring, int> m_nameToIndexMap;

        std::vector<uint8> m_bufferData;
    };

} // namespace blink
