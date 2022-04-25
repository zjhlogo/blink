
/*********************************************************************
 * \file   VulkanUniformBlock.h
 * \brief
 *
 * \author zjhlogo
 * \date   4/25 2022
 *********************************************************************/
#pragma once

#include <core/types/UniformTypes.h>
#include <foundation/BaseTypesGlm.h>

#include <vector>

namespace blink
{
    class VulkanUniformBlock
    {
    public:
        struct UniformMemberInfo
        {
            tstring name;
            UniformType type;
            uint32 offset;
        };

    public:
        VulkanUniformBlock() = default;
        ~VulkanUniformBlock() = default;

        void addUniformMember(const tstring& name, UniformType type);
        void setUniformMember(const tstring& name, UniformType type, const void* data);
        void setUniformMember(int memberIndex, UniformType type, const void* data);
        const tstring& getMemberName(int memberIndex);

    private:
        std::vector<UniformMemberInfo> m_memberInfoList;

    };

} // namespace blink
