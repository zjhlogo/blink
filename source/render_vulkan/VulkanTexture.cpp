/*!
 * \file VulkanTexture.cpp
 *
 * \author zjhlogo
 * \date 2020/01/19
 *
 *
 */
#include "VulkanTexture.h"
#include "VulkanRenderModule.h"

NS_BEGIN

VulkanTexture::VulkanTexture(VulkanRenderModule* renderModule)
    : Texture(EMPTY_STRING)
    , m_renderModule(renderModule)
{
}

VulkanTexture::~VulkanTexture()
{
}

bool VulkanTexture::createTexture2D(void* pixels, int width, int height, int channels)
{
    if (!createTextureImage(pixels, width, height, channels)) return false;

    m_textureImageView = createTextureImageView(m_textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor);
    if (!m_textureImageView) return false;

    if (!createTextureSampler()) return false;

    return true;
}

bool VulkanTexture::createDepthTexture(int width, int height)
{
    vk::Format depthFormat = findSupportedFormat({vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
                                                 vk::ImageTiling::eOptimal,
                                                 vk::FormatFeatureFlagBits::eDepthStencilAttachment);

    // create image
    {
        auto& logicalDevice = m_renderModule->getLogicalDevice();

        vk::ImageCreateInfo imageInfo;
        imageInfo.imageType = vk::ImageType::e2D;
        imageInfo.extent.width = static_cast<uint32_t>(width);
        imageInfo.extent.height = static_cast<uint32_t>(height);
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = depthFormat;
        imageInfo.tiling = vk::ImageTiling::eOptimal;
        imageInfo.initialLayout = vk::ImageLayout::eUndefined;
        imageInfo.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
        imageInfo.sharingMode = vk::SharingMode::eExclusive;
        imageInfo.samples = vk::SampleCountFlagBits::e1;
        m_textureImage = logicalDevice.createImage(imageInfo);

        vk::MemoryRequirements memRequirements = logicalDevice.getImageMemoryRequirements(m_textureImage);
        vk::MemoryAllocateInfo allocInfo;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
        m_textureImageMemory = logicalDevice.allocateMemory(allocInfo);

        logicalDevice.bindImageMemory(m_textureImage, m_textureImageMemory, 0);
    }

    m_textureImageView = createTextureImageView(m_textureImage, depthFormat, vk::ImageAspectFlagBits::eDepth);
    if (!m_textureImage) return false;

    transitionImageLayout(m_textureImage, depthFormat, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal);

    return true;
}

void VulkanTexture::destroy()
{
    destroyTextureSampler();
    destroyTextureImageView();
    destroyTextureImage();
}

bool VulkanTexture::createTextureImage(void* pixels, int width, int height, int channels)
{
    vk::DeviceSize imageSize = width * height * 4;
    auto& logicalDevice = m_renderModule->getLogicalDevice();

    // create staging buffer
    vk::Buffer stagingBuffer;
    vk::DeviceMemory stagingBufferMemory;
    {
        vk::BufferCreateInfo bufferInfo;
        bufferInfo.size = imageSize;
        bufferInfo.usage = vk::BufferUsageFlagBits::eTransferSrc;
        bufferInfo.sharingMode = vk::SharingMode::eExclusive;
        stagingBuffer = logicalDevice.createBuffer(bufferInfo);

        vk::MemoryRequirements memRequirements = logicalDevice.getBufferMemoryRequirements(stagingBuffer);

        vk::MemoryAllocateInfo allocInfo;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
                                                   vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
        stagingBufferMemory = logicalDevice.allocateMemory(allocInfo);
        logicalDevice.bindBufferMemory(stagingBuffer, stagingBufferMemory, 0);
    }

    // copy buffer into staging buffer memory
    {
        void* mapedBuffer = logicalDevice.mapMemory(stagingBufferMemory, 0, imageSize);
        memcpy(mapedBuffer, pixels, static_cast<size_t>(imageSize));
        logicalDevice.unmapMemory(stagingBufferMemory);
    }

    // create image
    {
        vk::ImageCreateInfo imageInfo;
        imageInfo.imageType = vk::ImageType::e2D;
        imageInfo.extent.width = static_cast<uint32_t>(width);
        imageInfo.extent.height = static_cast<uint32_t>(height);
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = vk::Format::eR8G8B8A8Unorm;
        imageInfo.tiling = vk::ImageTiling::eOptimal;
        imageInfo.initialLayout = vk::ImageLayout::eUndefined;
        imageInfo.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
        imageInfo.sharingMode = vk::SharingMode::eExclusive;
        imageInfo.samples = vk::SampleCountFlagBits::e1;
        m_textureImage = logicalDevice.createImage(imageInfo);

        vk::MemoryRequirements memRequirements = logicalDevice.getImageMemoryRequirements(m_textureImage);
        vk::MemoryAllocateInfo allocInfo;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
        m_textureImageMemory = logicalDevice.allocateMemory(allocInfo);

        logicalDevice.bindImageMemory(m_textureImage, m_textureImageMemory, 0);
    }

    transitionImageLayout(m_textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);

    // copy buffer to image
    {
        vk::CommandBuffer commandBuffer = beginSingleTimeCommands();

        vk::BufferImageCopy region;
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = {0, 0, 0};
        region.imageExtent = {(uint32_t)width, (uint32_t)height, 1};

        commandBuffer.copyBufferToImage(stagingBuffer, m_textureImage, vk::ImageLayout::eTransferDstOptimal, region);

        endSingleTimeCommands(commandBuffer);
    }

    transitionImageLayout(m_textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

    // destroy staging buffer
    logicalDevice.destroyBuffer(stagingBuffer);
    logicalDevice.freeMemory(stagingBufferMemory);

    return true;
}

void VulkanTexture::destroyTextureImage()
{
    auto& logicalDevice = m_renderModule->getLogicalDevice();
    logicalDevice.destroyImage(m_textureImage);
    logicalDevice.freeMemory(m_textureImageMemory);
}

vk::ImageView VulkanTexture::createTextureImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags)
{
    auto& logicalDevice = m_renderModule->getLogicalDevice();

    vk::ImageViewCreateInfo viewInfo;
    viewInfo.image = image;
    viewInfo.viewType = vk::ImageViewType::e2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;
    return logicalDevice.createImageView(viewInfo);
}

void VulkanTexture::destroyTextureImageView()
{
    auto& logicalDevice = m_renderModule->getLogicalDevice();
    logicalDevice.destroyImageView(m_textureImageView);
}

bool VulkanTexture::createTextureSampler()
{
    auto& logicalDevice = m_renderModule->getLogicalDevice();

    vk::SamplerCreateInfo samplerInfo;
    samplerInfo.magFilter = vk::Filter::eLinear;
    samplerInfo.minFilter = vk::Filter::eLinear;
    samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
    samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
    samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = 16;
    samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = vk::CompareOp::eAlways;
    samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;
    m_textureSampler = logicalDevice.createSampler(samplerInfo);

    return true;
}

void VulkanTexture::destroyTextureSampler()
{
    auto& logicalDevice = m_renderModule->getLogicalDevice();

    logicalDevice.destroySampler(m_textureSampler);
}

uint32_t VulkanTexture::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
{
    auto& physicalDevice = m_renderModule->getPhysicalDevice();

    vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
    {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) return i;
    }

    assert(false);
    return 0;
}

void VulkanTexture::transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
{
    vk::CommandBuffer commandBuffer = beginSingleTimeCommands();

    vk::ImageMemoryBarrier barrier;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;

    if (newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
    {
        barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;

        if (format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint)
        {
            barrier.subresourceRange.aspectMask |= vk::ImageAspectFlagBits::eStencil;
        }
    }
    else
    {
        barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    }

    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    vk::PipelineStageFlags sourceStage;
    vk::PipelineStageFlags destinationStage;

    if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
    {
        barrier.srcAccessMask = {};
        barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

        sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
        destinationStage = vk::PipelineStageFlagBits::eTransfer;
    }
    else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal)
    {
        barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

        sourceStage = vk::PipelineStageFlagBits::eTransfer;
        destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
    }
    else if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
    {
        barrier.srcAccessMask = {};
        barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;

        sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
        destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
    }

    commandBuffer.pipelineBarrier(sourceStage, destinationStage, {}, 0, nullptr, 0, nullptr, 1, &barrier);

    endSingleTimeCommands(commandBuffer);
}

vk::CommandBuffer VulkanTexture::beginSingleTimeCommands()
{
    auto& logicalDevice = m_renderModule->getLogicalDevice();
    auto& commandPool = m_renderModule->getCommandPool();

    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    vk::CommandBuffer commandBuffer;
    logicalDevice.allocateCommandBuffers(&allocInfo, &commandBuffer);

    vk::CommandBufferBeginInfo beginInfo;
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

    commandBuffer.begin(beginInfo);
    return commandBuffer;
}

void VulkanTexture::endSingleTimeCommands(vk::CommandBuffer commandBuffer)
{
    auto& logicalDevice = m_renderModule->getLogicalDevice();
    auto& commandPool = m_renderModule->getCommandPool();
    auto& graphicsQueue = m_renderModule->getGraphicsQueue();

    commandBuffer.end();

    vk::SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    graphicsQueue.submit(submitInfo, vk::Fence());
    graphicsQueue.waitIdle();
    logicalDevice.freeCommandBuffers(commandPool, commandBuffer);
}

vk::Format VulkanTexture::findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features)
{
    auto& physicalDevice = m_renderModule->getPhysicalDevice();

    for (vk::Format format : candidates)
    {
        vk::FormatProperties props = physicalDevice.getFormatProperties(format);
        if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    return vk::Format::eD24UnormS8Uint;
}

NS_END
