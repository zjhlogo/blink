/*!
 * \file VulkanFence.h
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

    class VulkanFence
    {
    public:
        VulkanFence(VulkanLogicalDevice& logicalDevice);
        ~VulkanFence();

        operator VkFence() { return m_fence; };

        bool create(bool signaled = false);
        void destroy();

        void wait();
        void reset();

    private:
        VulkanLogicalDevice& m_logicalDevice;

        VkFence m_fence{};
    };

} // namespace blink
