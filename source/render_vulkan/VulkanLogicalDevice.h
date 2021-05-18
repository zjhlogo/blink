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
#include <vulkan/vulkan.h>

NS_BEGIN

class VulkanContext;

class VulkanLogicalDevice
{
public:
    VulkanLogicalDevice();
    ~VulkanLogicalDevice();

    bool initialize(VulkanContext* context);
    void terminate();

    operator VkDevice () { return m_logicalDevice; };

    //vk::ImageView createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);

    //const vk::Device& getVkLogicalDevice() const { return m_logicalDevice; };
    //const vk::Queue& getGraphicsQueue() const { return m_graphicsQueue; };

    //bool createBuffer(vk::Buffer& buffer,
    //                  vk::DeviceMemory& bufferMemory,
    //                  const vk::DeviceSize& size,
    //                  vk::BufferUsageFlags usage,
    //                  vk::MemoryPropertyFlags properties);
    //void destroyBuffer(vk::Buffer buffer, vk::DeviceMemory bufferMemory);
    //void copyBuffer(vk::Buffer& srcBuffer, vk::Buffer& dstBuffer, const vk::DeviceSize& size, vk::CommandPool commandPool);

    //bool createVertexBuffer(vk::Buffer& vertexBufferOut, vk::DeviceMemory& vertexBufferMemoryOut, vk::CommandPool commandPool, const void* buffer, const vk::DeviceSize& size);
    //bool createIndexBuffer(vk::Buffer& indexBufferOut, vk::DeviceMemory& indexBufferMemoryOut, vk::CommandPool commandPool, const void* buffer, const vk::DeviceSize& size);

    //vk::Framebuffer createFramebuffer(vk::ImageView imageView, VulkanTexture* texture, vk::RenderPass renderPass, const vk::Extent2D& extent);
    //void destroyFramebuffer(vk::Framebuffer frameBuffer);

    VulkanContext* getContext() { return m_context; };
    
private:
    bool createLogicalDevice();
    void destroyLogicalDevice();

    //vk::CommandBuffer beginSingleTimeCommands(vk::CommandPool commandPool);
    //void endSingleTimeCommands(vk::CommandPool commandPool, vk::CommandBuffer commandBuffer);

private:
    VulkanContext* m_context{};

    VkDevice m_logicalDevice{};
    VkQueue m_graphicsQueue{};
    VkQueue m_presentQueue{};
};

NS_END
