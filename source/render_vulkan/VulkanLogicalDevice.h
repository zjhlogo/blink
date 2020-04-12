/*!
 * \file VulkanLogicalDevice.h
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.hpp>

NS_BEGIN

class VulkanContext;

class VulkanLogicalDevice
{
public:
    VulkanLogicalDevice();
    ~VulkanLogicalDevice();

    bool initialize(VulkanContext* context);
    void terminate();

    vk::ImageView createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);

    const vk::Device& getVkLogicalDevice() const { return m_logicalDevice; };
    const vk::Queue& getGraphicsQueue() const { return m_graphicsQueue; };

private:
    bool createLogicalDevice();
    void destroyLogicalDevice();

private:
    VulkanContext* m_context{};

    vk::Device m_logicalDevice;
    vk::Queue m_graphicsQueue;
    vk::Queue m_presentQueue;
};

NS_END
