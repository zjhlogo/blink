/*!
 * \file VulkanCommandPool.h
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#pragma once
#include "VulkanBase.h"

namespace blink
{
    class VulkanLogicalDevice;

    class VulkanCommandPool
    {
    public:
        explicit VulkanCommandPool(VulkanLogicalDevice& logicalDevice);
        ~VulkanCommandPool();

        VulkanCommandPool(const VulkanCommandPool&) = delete;
        VulkanCommandPool(VulkanCommandPool&&) = delete;
        VulkanCommandPool& operator=(const VulkanCommandPool&) = delete;
        VulkanCommandPool& operator=(VulkanCommandPool&&) = delete;

        explicit operator VkCommandPool() { return m_commandPool; }

        bool create();
        void destroy();

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VkCommandPool m_commandPool{};
    };
} // namespace blink
