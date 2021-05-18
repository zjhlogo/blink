/*!
 * \file VulkanLogicalDevice.cpp
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#include "VulkanLogicalDevice.h"
#include "VulkanContext.h"
#include "utils/VulkanUtils.h"

#include <foundation/Log.h>

#include <set>

NS_BEGIN

VulkanLogicalDevice::VulkanLogicalDevice()
{
}

VulkanLogicalDevice::~VulkanLogicalDevice()
{
    destroyLogicalDevice();
}

bool VulkanLogicalDevice::initialize(VulkanContext* context)
{
    m_context = context;

    if (!createLogicalDevice()) return false;
    return true;
}

void VulkanLogicalDevice::terminate()
{
    destroyLogicalDevice();
}

// bool VulkanLogicalDevice::createBuffer(vk::Buffer& buffer,
//                                       vk::DeviceMemory& bufferMemory,
//                                       const vk::DeviceSize& size,
//                                       vk::BufferUsageFlags usage,
//                                       vk::MemoryPropertyFlags properties)
//{
//    vk::BufferCreateInfo bufferInfo;
//    bufferInfo.size = size;
//    bufferInfo.usage = usage;
//    bufferInfo.sharingMode = vk::SharingMode::eExclusive;
//    buffer = m_logicalDevice.createBuffer(bufferInfo);
//
//    vk::MemoryRequirements memRequirements = m_logicalDevice.getBufferMemoryRequirements(buffer);
//
//    vk::MemoryAllocateInfo allocInfo;
//    allocInfo.allocationSize = memRequirements.size;
//    allocInfo.memoryTypeIndex = VulkanUtils::findMemoryType(m_context->getMemoryProperties(), memRequirements.memoryTypeBits, properties);
//    bufferMemory = m_logicalDevice.allocateMemory(allocInfo);
//    m_logicalDevice.bindBufferMemory(buffer, bufferMemory, 0);
//
//    return true;
//}
//
// void VulkanLogicalDevice::destroyBuffer(vk::Buffer buffer, vk::DeviceMemory bufferMemory)
//{
//    m_logicalDevice.destroyBuffer(buffer);
//    m_logicalDevice.freeMemory(bufferMemory);
//}
//
// void VulkanLogicalDevice::copyBuffer(vk::Buffer& srcBuffer, vk::Buffer& dstBuffer, const vk::DeviceSize& size, vk::CommandPool commandPool)
//{
//    vk::CommandBuffer commandBuffer = beginSingleTimeCommands(commandPool);
//
//    vk::BufferCopy copyRegion;
//    copyRegion.srcOffset = 0;
//    copyRegion.dstOffset = 0;
//    copyRegion.size = size;
//    commandBuffer.copyBuffer(srcBuffer, dstBuffer, copyRegion);
//
//    endSingleTimeCommands(commandPool, commandBuffer);
//}
//
// bool VulkanLogicalDevice::createVertexBuffer(vk::Buffer& vertexBufferOut,
//                                             vk::DeviceMemory& vertexBufferMemoryOut,
//                                             vk::CommandPool commandPool,
//                                             const void* buffer,
//                                             const vk::DeviceSize& size)
//{
//    vk::Buffer stagingBuffer;
//    vk::DeviceMemory stagingBufferMemory;
//
//    createBuffer(stagingBuffer,
//                 stagingBufferMemory,
//                 size,
//                 vk::BufferUsageFlagBits::eTransferSrc,
//                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
//
//    void* data = m_logicalDevice.mapMemory(stagingBufferMemory, 0, size);
//    memcpy(data, buffer, static_cast<size_t>(size));
//    m_logicalDevice.unmapMemory(stagingBufferMemory);
//
//    createBuffer(vertexBufferOut,
//                 vertexBufferMemoryOut,
//                 size,
//                 vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
//                 vk::MemoryPropertyFlagBits::eDeviceLocal);
//
//    copyBuffer(stagingBuffer, vertexBufferOut, size, commandPool);
//
//    m_logicalDevice.destroyBuffer(stagingBuffer);
//    m_logicalDevice.freeMemory(stagingBufferMemory);
//
//    return true;
//}
//
// bool VulkanLogicalDevice::createIndexBuffer(vk::Buffer& indexBufferOut,
//                                            vk::DeviceMemory& indexBufferMemoryOut,
//                                            vk::CommandPool commandPool,
//                                            const void* buffer,
//                                            const vk::DeviceSize& size)
//{
//    vk::Buffer stagingBuffer;
//    vk::DeviceMemory stagingBufferMemory;
//    createBuffer(stagingBuffer,
//                 stagingBufferMemory,
//                 size,
//                 vk::BufferUsageFlagBits::eTransferSrc,
//                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
//
//    void* data = m_logicalDevice.mapMemory(stagingBufferMemory, 0, size);
//    memcpy(data, buffer, static_cast<size_t>(size));
//    m_logicalDevice.unmapMemory(stagingBufferMemory);
//
//    createBuffer(indexBufferOut,
//                 indexBufferMemoryOut,
//                 size,
//                 vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
//                 vk::MemoryPropertyFlagBits::eDeviceLocal);
//    copyBuffer(stagingBuffer, indexBufferOut, size, commandPool);
//
//    m_logicalDevice.destroyBuffer(stagingBuffer);
//    m_logicalDevice.freeMemory(stagingBufferMemory);
//
//    return true;
//}
//
// vk::Framebuffer VulkanLogicalDevice::createFramebuffer(vk::ImageView imageView, VulkanTexture* texture, vk::RenderPass renderPass, const vk::Extent2D&
// extent)
//{
//    std::array<vk::ImageView, 2> attacments = {imageView, texture->getTextureImageView()};
//
//    vk::FramebufferCreateInfo frameBufferInfo;
//    frameBufferInfo.renderPass = renderPass;
//    frameBufferInfo.attachmentCount = static_cast<uint32_t>(attacments.size());
//    frameBufferInfo.pAttachments = attacments.data();
//    frameBufferInfo.width = extent.width;
//    frameBufferInfo.height = extent.height;
//    frameBufferInfo.layers = 1;
//
//    return m_logicalDevice.createFramebuffer(frameBufferInfo);
//}
//
// void VulkanLogicalDevice::destroyFramebuffer(vk::Framebuffer frameBuffer)
//{
//    m_logicalDevice.destroyFramebuffer(frameBuffer);
//}
//
// vk::ImageView VulkanLogicalDevice::createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags)
//{
//    vk::ImageViewCreateInfo viewInfo;
//    viewInfo.image = image;
//    viewInfo.viewType = vk::ImageViewType::e2D;
//    viewInfo.format = format;
//    viewInfo.subresourceRange.aspectMask = aspectFlags;
//    viewInfo.subresourceRange.baseMipLevel = 0;
//    viewInfo.subresourceRange.levelCount = 1;
//    viewInfo.subresourceRange.baseArrayLayer = 0;
//    viewInfo.subresourceRange.layerCount = 1;
//    return m_logicalDevice.createImageView(viewInfo);
//}

bool VulkanLogicalDevice::createLogicalDevice()
{
    // get best fit queue index from queue families
    int graphicsFamilyIndex{};
    int presentFamilyIndex{};
    if (!VulkanUtils::getBestFitQueueFamilyPropertyIndex(graphicsFamilyIndex,
                                                         presentFamilyIndex,
                                                         m_context->getPickedPhysicalDevice(),
                                                         m_context->getVkSurface(),
                                                         m_context->getQueueFamilyProperties()))
    {
        return false;
    }

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> uniqueQueueFamilies = {graphicsFamilyIndex, presentFamilyIndex};

    float priority = 1.0f;
    for (uint32_t queueFamilyIndex : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &priority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.enabledExtensionCount = 0;

    if (VulkanUtils::checkValidationLayerSupported(m_context->getLayerProperties(), VulkanUtils::getRequiredValidationLayers()))
    {
        const auto& requiredLayers = VulkanUtils::getRequiredValidationLayers();
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
        deviceCreateInfo.ppEnabledLayerNames = requiredLayers.data();
    }

    if (VulkanUtils::checkExtensionsSupported(m_context->getExtensionProperties(), VulkanUtils::getRequiredDeviceExtensions()))
    {
        const auto& requiredExtensions = VulkanUtils::getRequiredDeviceExtensions();
        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
    }

    if (vkCreateDevice(m_context->getPickedPhysicalDevice(), &deviceCreateInfo, nullptr, &m_logicalDevice) != VK_SUCCESS)
    {
        LOGE("create device failed");
        return false;
    }

    // retriveing queue handles
    vkGetDeviceQueue(m_logicalDevice, graphicsFamilyIndex, 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_logicalDevice, presentFamilyIndex, 0, &m_presentQueue);

    return true;
}

void VulkanLogicalDevice::destroyLogicalDevice()
{
    vkDestroyDevice(m_logicalDevice, nullptr);
}

//vk::CommandBuffer VulkanLogicalDevice::beginSingleTimeCommands(vk::CommandPool commandPool)
//{
//    vk::CommandBufferAllocateInfo allocInfo;
//    allocInfo.level = vk::CommandBufferLevel::ePrimary;
//    allocInfo.commandPool = commandPool;
//    allocInfo.commandBufferCount = 1;
//
//    vk::CommandBuffer commandBuffer;
//    m_logicalDevice.allocateCommandBuffers(&allocInfo, &commandBuffer);
//
//    vk::CommandBufferBeginInfo beginInfo;
//    beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
//
//    commandBuffer.begin(beginInfo);
//    return commandBuffer;
//}
//
//void VulkanLogicalDevice::endSingleTimeCommands(vk::CommandPool commandPool, vk::CommandBuffer commandBuffer)
//{
//    commandBuffer.end();
//
//    vk::SubmitInfo submitInfo;
//    submitInfo.commandBufferCount = 1;
//    submitInfo.pCommandBuffers = &commandBuffer;
//
//    m_graphicsQueue.submit(submitInfo, vk::Fence());
//    m_graphicsQueue.waitIdle();
//    m_logicalDevice.freeCommandBuffers(commandPool, commandBuffer);
//}

NS_END
