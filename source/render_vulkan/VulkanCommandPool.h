/*!
 * \file VulkanCommandPool.h
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <vulkan/vulkan.h>

#include <vector>

namespace blink
{
    class VulkanLogicalDevice;

    class VulkanCommandPool
    {
    public:
        VulkanCommandPool(VulkanLogicalDevice& logicalDevice);
        ~VulkanCommandPool();

        operator VkCommandPool() { return m_commandPool; };

        bool create();
        void destroy();

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VkCommandPool m_commandPool{};
    };

} // namespace blink
