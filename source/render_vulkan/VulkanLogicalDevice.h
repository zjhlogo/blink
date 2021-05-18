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

    bool createBuffer(vk::Buffer& buffer,
                      vk::DeviceMemory& bufferMemory,
                      const vk::DeviceSize& size,
                      vk::BufferUsageFlags usage,
                      vk::MemoryPropertyFlags properties);
    void destroyBuffer(vk::Buffer buffer, vk::DeviceMemory bufferMemory);
    void copyBuffer(vk::Buffer& srcBuffer, vk::Buffer& dstBuffer, const vk::DeviceSize& size, vk::CommandPool commandPool);

    bool createVertexBuffer(vk::Buffer& vertexBufferOut, vk::DeviceMemory& vertexBufferMemoryOut, vk::CommandPool commandPool, const void* buffer, const vk::DeviceSize& size);
    bool createIndexBuffer(vk::Buffer& indexBufferOut, vk::DeviceMemory& indexBufferMemoryOut, vk::CommandPool commandPool, const void* buffer, const vk::DeviceSize& size);

private:
    bool createLogicalDevice();
    void destroyLogicalDevice();

    vk::CommandBuffer beginSingleTimeCommands(vk::CommandPool commandPool);
    void endSingleTimeCommands(vk::CommandPool commandPool, vk::CommandBuffer commandBuffer);

private:
    VulkanContext* m_context{};

    vk::Device m_logicalDevice;
    vk::Queue m_graphicsQueue;
    vk::Queue m_presentQueue;
};

NS_END
