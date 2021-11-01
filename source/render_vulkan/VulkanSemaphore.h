/*!
 * \file VulkanSemaphore.h
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <vulkan/vulkan.h>

namespace blink
{
    class VulkanLogicalDevice;

    class VulkanSemaphore
    {
    public:
        VulkanSemaphore(VulkanLogicalDevice& logicalDevice);
        ~VulkanSemaphore();

        operator VkSemaphore() { return m_semaphore; };

        bool create();
        void destroy();

    private:
        VulkanLogicalDevice& m_logicalDevice;

        VkSemaphore m_semaphore{};
    };

} // namespace blink
