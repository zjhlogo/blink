/*!
 * \file VulkanSemaphore.h
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

    class VulkanSemaphore
    {
    public:
        explicit VulkanSemaphore(VulkanLogicalDevice& logicalDevice);
        ~VulkanSemaphore();

        VulkanSemaphore(const VulkanSemaphore& semaphore) = delete;
        VulkanSemaphore(VulkanSemaphore&& semaphore) = delete;
        VulkanSemaphore& operator=(const VulkanSemaphore& semaphore) = delete;
        VulkanSemaphore& operator=(VulkanSemaphore&& semaphore) = delete;

        operator VkSemaphore() { return m_semaphore; }

        bool create();
        void destroy();

    private:
        VulkanLogicalDevice& m_logicalDevice;

        VkSemaphore m_semaphore{};
    };
} // namespace blink
