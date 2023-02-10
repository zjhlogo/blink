/*********************************************************************
 * \file   VulkanUniformBlock.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   4/25 2022
 *********************************************************************/
#include "VulkanUniformBlock.h"

namespace blink
{
    void VulkanUniformBlock::reset()
    {
        m_uniformStructSize = 0;
        m_memberInfoList.clear();
        m_nameToIndexMap.clear();
    }

    void VulkanUniformBlock::prepareBuffer()
    {
        //
        if (m_bufferData.size() < m_uniformStructSize) m_bufferData.resize(m_uniformStructSize);
    }

    bool VulkanUniformBlock::isReady()
    {
        //
        return !m_bufferData.empty();
    }


    template <> bool VulkanUniformBlock::setUniformMember(int memberIndex, const glm::mat3* value)
    {
        const auto& memberInfo = m_memberInfoList[memberIndex];
        if (UniformType::Mat3 != memberInfo.type) return false;

        auto pData1 = reinterpret_cast<glm::vec3*>(m_bufferData.data() + memberInfo.offset);
        auto pData2 = reinterpret_cast<glm::vec3*>(m_bufferData.data() + memberInfo.offset + 16);
        auto pData3 = reinterpret_cast<glm::vec3*>(m_bufferData.data() + memberInfo.offset + 32);

        *pData1 = (*value)[0];
        *pData2 = (*value)[1];
        *pData3 = (*value)[2];
        return true;
    }

    template <> bool VulkanUniformBlock::getUniformMember(glm::mat3* valueOut, int memberIndex)
    {
        const auto& memberInfo = m_memberInfoList[memberIndex];
        if (UniformType::Mat3 != memberInfo.type) return false;

        (*valueOut)[0] = *reinterpret_cast<glm::vec3*>(m_bufferData.data() + memberInfo.offset);
        (*valueOut)[1] = *reinterpret_cast<glm::vec3*>(m_bufferData.data() + memberInfo.offset + 16);
        (*valueOut)[2] = *reinterpret_cast<glm::vec3*>(m_bufferData.data() + memberInfo.offset + 32);

        return true;
    }

    bool VulkanUniformBlock::addUniformMember(const tstring& name, UniformType type, uint32_t offset)
    {
        auto it = m_nameToIndexMap.find(name);
        if (it != m_nameToIndexMap.end())
        {
            // duplicate member name
            return false;
        }

        m_nameToIndexMap.insert({name, static_cast<int>(m_memberInfoList.size())});

        // auto offset = calculateOffset(type);
        m_uniformStructSize = offset + calculateSize(type);
        m_memberInfoList.push_back({name, type, offset});

        return true;
    }

    const tstring& VulkanUniformBlock::getMemberName(int memberIndex) const
    {
        //
        return m_memberInfoList[memberIndex].name;
    }

    UniformType VulkanUniformBlock::getMemberType(int memberIndex) const
    {
        //
        return m_memberInfoList[memberIndex].type;
    }

    uint32_t VulkanUniformBlock::calculateSize(UniformType type)
    {
        static const uint32_t SIZE_MAP[static_cast<int>(UniformType::NumTypes)] =
        {
            0,            // None
            4,            // Int
            4,            // Float
            8,            // Vec2
            16,           // Vec3
            16,           // Vec4
            48,           // Mat3
            64,           // Mat4
        };

        return SIZE_MAP[static_cast<int>(type)];
    }

    uint32_t VulkanUniformBlock::calculateOffset(UniformType type) const
    {
        static const uint32_t OFFSET_MAP[static_cast<int>(UniformType::NumTypes)] =
        {
            0,            // None
            4,            // Int
            4,            // Float
            8,            // Vec2
            16,           // Vec3
            16,           // Vec4
            16,           // Mat3
            16,           // Mat4
        };

        auto align = OFFSET_MAP[static_cast<int>(type)];
        auto alignedStructSize = (m_uniformStructSize + align - 1) / align * align;
        return alignedStructSize;
    }
} // namespace blink
