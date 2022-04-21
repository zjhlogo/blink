
/*********************************************************************
 * \file   VulkanGeometry.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/15/2022
 *********************************************************************/
#pragma once

#include "../VulkanBuffer.h"

#include <core/resources/IGeometry.h>
#include <vulkan/vulkan.h>

namespace blink
{
    class VulkanGeometry : public IGeometry
    {
    public:
        virtual VulkanBuffer* getVulkanBuffer() const = 0;
        virtual VkDeviceSize getVertexInputOffset(uint32 inputMask) const = 0;
        virtual VkDeviceSize getIndicesOffset() const = 0;
    };
} // namespace blink
