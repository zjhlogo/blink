/*!
 * \file VulkanTexture.cpp
 *
 * \author zjhlogo
 * \date 2020/01/19
 *
 *
 */
#include "VulkanTexture.h"
#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanContext.h"
#include "VulkanImage.h"
#include "VulkanLogicalDevice.h"
#include "VulkanMemory.h"
#include "utils/VulkanUtils.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tinygltf/tiny_gltf.h>

namespace blink
{
    VulkanTexture::VulkanTexture(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& pool)
        : m_logicalDevice(logicalDevice)
        , m_commandPool(pool)
    {
    }

    VulkanTexture::~VulkanTexture() { destroy(); }

    bool VulkanTexture::createTexture2D(const tstring& texFile)
    {
        int texWidth = 0;
        int texHeight = 0;
        int texChannels = 0;

        stbi_uc* pixels = stbi_load(texFile.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        if (!pixels) return nullptr;

        bool success = createTexture2D(pixels, texWidth, texHeight, texChannels);
        stbi_image_free(pixels);

        return success;
    }

    bool VulkanTexture::createTexture2D(void* pixels, int width, int height, int channels)
    {
        if (!createTextureImage(pixels, width, height, channels)) return false;

        if (m_textureImage->createImageView(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT) == nullptr)
        {
            return false;
        }

        if (!createTextureSampler()) return false;

        return true;
    }

    bool VulkanTexture::createDepthTexture(int width, int height)
    {
        destroyTextureImage();

        VkFormat depthFormat = VulkanUtils::findSupportedFormat(m_logicalDevice.getContext()->getPickedPhysicalDevice(),
                                                                {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                                                                VK_IMAGE_TILING_OPTIMAL,
                                                                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

        // create depth image
        m_textureImage = new VulkanImage(m_logicalDevice);
        m_textureImage->createImage(VK_IMAGE_TYPE_2D, width, height, depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
        m_textureImage->allocateImageMemory();
        m_textureImage->createImageView(depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
        m_textureImage->transitionImageLayout(m_commandPool, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        return true;
    }

    void VulkanTexture::destroy()
    {
        destroyTextureSampler();
        destroyTextureImage();
    }

    VulkanImage* VulkanTexture::createTextureImage(void* pixels, int width, int height, int channels)
    {
        destroyTextureImage();

        VkDeviceSize imageSize = width * height * 4;

        // create staging buffer
        VulkanBuffer* stagingBuffer = new VulkanBuffer(m_logicalDevice);
        stagingBuffer->createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
        VulkanMemory* bufferMemory = stagingBuffer->allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        // copy buffer into staging buffer memory
        bufferMemory->uploadData(pixels, imageSize, 0);

        // create image
        m_textureImage = new VulkanImage(m_logicalDevice);
        m_textureImage->createImage(VK_IMAGE_TYPE_2D, width, height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
        m_textureImage->allocateImageMemory();

        m_textureImage->transitionImageLayout(m_commandPool, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        // copy buffer to image
        {
            VulkanCommandBuffer commandBuffer(m_logicalDevice, m_commandPool);
            commandBuffer.create();
            commandBuffer.beginCommand();

            VkBufferImageCopy region;
            region.bufferOffset = 0;
            region.bufferRowLength = 0;
            region.bufferImageHeight = 0;

            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.mipLevel = 0;
            region.imageSubresource.baseArrayLayer = 0;
            region.imageSubresource.layerCount = 1;

            region.imageOffset = {0, 0, 0};
            region.imageExtent = {(uint32_t)width, (uint32_t)height, 1};

            vkCmdCopyBufferToImage(commandBuffer, *stagingBuffer, *m_textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

            commandBuffer.endCommand();
            commandBuffer.submitCommand();
            commandBuffer.destroy();
        }

        m_textureImage->transitionImageLayout(m_commandPool,
                                              VK_FORMAT_R8G8B8A8_UNORM,
                                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        // destroy staging buffer
        SAFE_DELETE(stagingBuffer);

        return m_textureImage;
    }

    void VulkanTexture::destroyTextureImage() { SAFE_DELETE(m_textureImage); }

    VkSampler VulkanTexture::createTextureSampler()
    {
        destroyTextureSampler();

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = 16;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;
        vkCreateSampler(m_logicalDevice, &samplerInfo, nullptr, &m_textureSampler);

        return m_textureSampler;
    }

    void VulkanTexture::destroyTextureSampler()
    {
        if (m_textureSampler != nullptr)
        {
            vkDestroySampler(m_logicalDevice, m_textureSampler, nullptr);
            m_textureSampler = nullptr;
        }
    }

} // namespace blink
