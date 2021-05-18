/*!
 * \file VulkanDescriptorPool.h
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 * 
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <vulkan/vulkan.h>

NS_BEGIN

class VulkanLogicalDevice;

class VulkanDescriptorPool
{
public:
    VulkanDescriptorPool(VulkanLogicalDevice& logicalDevice);
    ~VulkanDescriptorPool();

    operator VkDescriptorPool() { return m_descriptorPool; };

    bool create(uint32_t count);
    void destroy();

private:
    VulkanLogicalDevice& m_logicalDevice;
    VkDescriptorPool m_descriptorPool{};

};

NS_END
