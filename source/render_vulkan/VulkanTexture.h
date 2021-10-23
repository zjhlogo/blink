/*!
 * \file VulkanTexture.h
 *
 * \author zjhlogo
 * \date 2020/01/19
 *
 *
 */
#pragma once

#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.h>

#include <vector>

NS_BEGIN

class VulkanImage;
class VulkanLogicalDevice;
class VulkanCommandPool;

class VulkanTexture
{
public:
    VulkanTexture(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& pool);
    virtual ~VulkanTexture();

    bool createTexture2D(const tstring& texFile);
    bool createTexture2D(void* pixels, int width, int height, int channels);
    bool createDepthTexture(int width, int height);

    void destroy();

    VulkanImage* getTextureImage() { return m_textureImage; };
    VkSampler getTextureSampler() { return m_textureSampler; };

private:
    VulkanImage* createTextureImage(void* pixels, int width, int height, int channels);
    void destroyTextureImage();

    VkSampler createTextureSampler();
    void destroyTextureSampler();

private:
    VulkanLogicalDevice& m_logicalDevice;
    VulkanCommandPool& m_commandPool;

    VulkanImage* m_textureImage{};
    VkSampler m_textureSampler{};
};

NS_END
