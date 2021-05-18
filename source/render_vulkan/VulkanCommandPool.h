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
#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.hpp>

NS_BEGIN

class VulkanContext;

class VulkanCommandPool
{
public:
    VulkanCommandPool(VulkanContext* context);
    ~VulkanCommandPool();

private:
    vk::CommandPool m_commandPool;

};

NS_END
