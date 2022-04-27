
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
        if (m_bufferData.size() < m_uniformStructSize)
        {
            m_bufferData.resize(m_uniformStructSize);
        }
    }

    bool VulkanUniformBlock::isReady()
    {
        //
        return !m_bufferData.empty();
    }

    bool VulkanUniformBlock::addUniformMember(const tstring& name, UniformType type, uint32 offset)
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

    uint32 VulkanUniformBlock::calculateSize(UniformType type) const
    {
        static const uint32 s_sizeMap[static_cast<int>(UniformType::NumTypes)] = {
            0,  // None
            4,  // Int
            4,  // Float
            8,  // Vec2
            16, // Vec3
            16, // Vec4
            48, // Mat3
            64, // Mat4
        };

        return s_sizeMap[static_cast<int>(type)];
    }

    uint32 VulkanUniformBlock::calculateOffset(UniformType type) const
    {
        static const uint32 s_offsetMap[static_cast<int>(UniformType::NumTypes)] = {
            0,  // None
            4,  // Int
            4,  // Float
            8,  // Vec2
            16, // Vec3
            16, // Vec4
            16, // Mat3
            16, // Mat4
        };

        auto align = s_offsetMap[static_cast<int>(type)];
        auto alignedStructSize = (m_uniformStructSize + align - 1) / align * align;
        return alignedStructSize;
    }
} // namespace blink
