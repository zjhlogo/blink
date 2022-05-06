/*!
 * \file VulkanSemaphore.cpp
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#include "VulkanSemaphore.h"
#include "VulkanLogicalDevice.h"

namespace blink
{
    VulkanSemaphore::VulkanSemaphore(VulkanLogicalDevice& logicalDevice)
        : m_logicalDevice(logicalDevice)
    {
        //
    }

    VulkanSemaphore::~VulkanSemaphore()
    {
        //
        destroy();
    }

    bool VulkanSemaphore::create()
    {
        VkSemaphoreCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VK_CHECK_RESULT(vkCreateSemaphore(m_logicalDevice, &info, nullptr, &m_semaphore));
        return true;
    }

    void VulkanSemaphore::destroy()
    {
        if (m_semaphore != nullptr)
        {
            vkDestroySemaphore(m_logicalDevice, m_semaphore, nullptr);
            m_semaphore = nullptr;
        }
    }

} // namespace blink
