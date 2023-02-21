/*!
 * \file VulkanTexture.cpp
 *
 * \author zjhlogo
 * \date 2020/01/19
 *
 *
 */
#include "VulkanTexture.h"
#include "../VulkanBuffer.h"
#include "../VulkanCommandBuffer.h"
#include "../VulkanContext.h"
#include "../VulkanImage.h"
#include "../VulkanLogicalDevice.h"
#include "../utils/VulkanUtils.h"

#include <foundation/File.h>

#define STB_IMAGE_IMPLEMENTATION
#include <tinygltf/stb_image.h>

namespace blink
{
    VulkanTexture::VulkanTexture(VulkanLogicalDevice& logicalDevice)
        : m_logicalDevice(logicalDevice)
    {
        //
    }

    VulkanTexture::~VulkanTexture()
    {
        //
        destroy();
    }

    bool VulkanTexture::createTexture2D(const tstring& texFile)
    {
        int texWidth = 0;
        int texHeight = 0;
        int texChannels = 0;

        std::vector<uint8_t> buffer;
        if (!File::readFileIntoBuffer(buffer, texFile))
        {
            LOGE("Open file failed {0}", texFile);
            return false;
        }

        stbi_uc* pixels = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size()), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        if (!pixels)
        {
            return false;
        }

        bool success = createTexture2D(pixels, texWidth, texHeight, true);
        stbi_image_free(pixels);

        return success;
    }

    bool VulkanTexture::createTexture2D(const void* pixels, uint32_t width, uint32_t height, bool generateMipMap)
    {
        if (!createTextureImage(pixels, width, height, generateMipMap))
        {
            return false;
        }

        if (m_textureImage->createImageView(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT) == VK_NULL_HANDLE)
        {
            return false;
        }

        if (!createTextureSampler())
        {
            return false;
        }

        return true;
    }

    bool VulkanTexture::createDepthTexture(uint32_t width, uint32_t height)
    {
        destroyTextureImage();

        VkFormat depthFormat = VulkanUtils::findSupportedFormat(m_logicalDevice.getContext()->getPickedPhysicalDevice(),
                                                                {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                                                                VK_IMAGE_TILING_OPTIMAL,
                                                                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

        // create depth image
        m_textureImage = new VulkanImage(m_logicalDevice, false);
        m_textureImage->createImage(VK_IMAGE_TYPE_2D, width, height, depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
        m_textureImage->allocateImageMemory();
        m_logicalDevice.executeCommand([&](const VulkanCommandBuffer& commandBuffer) {
            // transition to depth stencil attachment layout
            m_textureImage->transitionImageLayout(commandBuffer,
                                                  depthFormat,
                                                  VK_IMAGE_LAYOUT_UNDEFINED,
                                                  VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                                  0,
                                                  VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                                                  VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                                  VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                                                  0);
        });

        m_textureImage->createImageView(depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

        return true;
    }

    void VulkanTexture::destroy()
    {
        destroyTextureSampler();
        destroyTextureImage();
    }

    VulkanImage* VulkanTexture::createTextureImage(const void* pixels, uint32_t width, uint32_t height, bool generateMipMap)
    {
        destroyTextureImage();

        VkDeviceSize imageSize = 4ull * width * height;

        // create staging buffer
        auto stagingBuffer = new VulkanBuffer(m_logicalDevice);
        stagingBuffer->createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
        VulkanMemory* bufferMemory = stagingBuffer->allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        // copy buffer into staging buffer memory
        bufferMemory->uploadData(pixels, imageSize, 0);

        // create image
        m_textureImage = new VulkanImage(m_logicalDevice, generateMipMap);
        m_textureImage->createImage(VK_IMAGE_TYPE_2D,
                                    width,
                                    height,
                                    VK_FORMAT_R8G8B8A8_UNORM,
                                    VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
        uint32_t mipCount = m_textureImage->getMipCount();
        m_textureImage->allocateImageMemory();

        m_logicalDevice.executeCommand([&](const VulkanCommandBuffer& commandBuffer) {
            // transition layout to dst optimal
            m_textureImage->transitionImageLayout(commandBuffer,
                                                  VK_FORMAT_R8G8B8A8_UNORM,
                                                  VK_IMAGE_LAYOUT_UNDEFINED,
                                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                                  0,
                                                  VK_ACCESS_TRANSFER_WRITE_BIT,
                                                  VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                                  VK_PIPELINE_STAGE_TRANSFER_BIT,
                                                  0);
            // copy buffer to image
            m_textureImage->copyBufferToImage(commandBuffer, *stagingBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

            if (mipCount == 1)
            {
                m_textureImage->transitionImageLayout(commandBuffer,
                                                      VK_FORMAT_R8G8B8A8_UNORM,
                                                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                                      VK_ACCESS_TRANSFER_WRITE_BIT,
                                                      VK_ACCESS_SHADER_READ_BIT,
                                                      VK_PIPELINE_STAGE_TRANSFER_BIT,
                                                      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                                      0);
            }
            else
            {
                m_textureImage->transitionImageLayout(commandBuffer,
                                                      VK_FORMAT_UNDEFINED,
                                                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                                      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                                      VK_ACCESS_TRANSFER_WRITE_BIT,
                                                      VK_ACCESS_TRANSFER_READ_BIT,
                                                      VK_PIPELINE_STAGE_TRANSFER_BIT,
                                                      VK_PIPELINE_STAGE_TRANSFER_BIT,
                                                      0);
            }
        });

        // destroy staging buffer
        SAFE_DELETE(stagingBuffer);

        if (mipCount > 1)
        {
            m_logicalDevice.executeCommand([&](const VulkanCommandBuffer& commandBuffer) {
                for (uint32_t i = 1; i < mipCount; ++i)
                {
                    VkImageBlit imageBlit{};

                    // source
                    imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    imageBlit.srcSubresource.layerCount = 1;
                    imageBlit.srcSubresource.mipLevel = i - 1;
                    imageBlit.srcOffsets[1].x = static_cast<int32_t>(width >> (i - 1));
                    imageBlit.srcOffsets[1].y = static_cast<int32_t>(height >> (i - 1));
                    imageBlit.srcOffsets[1].z = 1;

                    // destination
                    imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    imageBlit.dstSubresource.layerCount = 1;
                    imageBlit.dstSubresource.mipLevel = i;
                    imageBlit.dstOffsets[1].x = static_cast<int32_t>(width >> i);
                    imageBlit.dstOffsets[1].y = static_cast<int32_t>(height >> i);
                    imageBlit.dstOffsets[1].z = 1;

                    // prepare current mip level as image blit destination
                    m_textureImage->transitionImageLayout(commandBuffer,
                                                          VK_FORMAT_UNDEFINED,
                                                          VK_IMAGE_LAYOUT_UNDEFINED,
                                                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                                          0,
                                                          VK_ACCESS_TRANSFER_WRITE_BIT,
                                                          VK_PIPELINE_STAGE_TRANSFER_BIT,
                                                          VK_PIPELINE_STAGE_TRANSFER_BIT,
                                                          i);
                    // blit image
                    vkCmdBlitImage(commandBuffer,
                                   *m_textureImage,
                                   VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                   *m_textureImage,
                                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                   1,
                                   &imageBlit,
                                   VK_FILTER_LINEAR);

                    // prepare current mip level as image libt sorce for next level
                    m_textureImage->transitionImageLayout(commandBuffer,
                                                          VK_FORMAT_UNDEFINED,
                                                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                                          VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                                          VK_ACCESS_TRANSFER_WRITE_BIT,
                                                          VK_ACCESS_TRANSFER_READ_BIT,
                                                          VK_PIPELINE_STAGE_TRANSFER_BIT,
                                                          VK_PIPELINE_STAGE_TRANSFER_BIT,
                                                          i);
                }

                m_textureImage->transitionImageLayout(commandBuffer,
                                                      VK_FORMAT_R8G8B8A8_UNORM,
                                                      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                                      VK_ACCESS_TRANSFER_READ_BIT,
                                                      VK_ACCESS_SHADER_READ_BIT,
                                                      VK_PIPELINE_STAGE_TRANSFER_BIT,
                                                      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                                      0,
                                                      mipCount);
            });
        }

        return m_textureImage;
    }

    void VulkanTexture::destroyTextureImage()
    {
        //
        SAFE_DELETE(m_textureImage);
    }

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
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1.0f;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = static_cast<float>(m_textureImage->getMipCount());
        vkCreateSampler(m_logicalDevice, &samplerInfo, nullptr, &m_textureSampler);

        return m_textureSampler;
    }

    void VulkanTexture::destroyTextureSampler()
    {
        if (m_textureSampler != VK_NULL_HANDLE)
        {
            vkDestroySampler(m_logicalDevice, m_textureSampler, nullptr);
            m_textureSampler = VK_NULL_HANDLE;
        }
    }
} // namespace blink
