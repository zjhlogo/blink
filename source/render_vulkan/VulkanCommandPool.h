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

        VulkanCommandPool(const VulkanCommandPool& pool) = delete;
        VulkanCommandPool(VulkanCommandPool&& pool) = delete;
        VulkanCommandPool& operator=(const VulkanCommandPool& pool) = delete;
        VulkanCommandPool& operator=(VulkanCommandPool&& pool) = delete;

        operator VkCommandPool() { return m_commandPool; }

        bool create();
        void destroy();

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VkCommandPool m_commandPool{};
    };
} // namespace blink
