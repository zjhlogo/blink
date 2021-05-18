/*!
 * \file VulkanTexture.h
 *
 * \author zjhlogo
 * \date 2020/01/19
 *
 *
 */
#pragma once

#include <render_base/Texture.h>
#include <vulkan/vulkan.h>

NS_BEGIN

class VulkanImage;

class VulkanTexture : public Texture
{
public:
    VulkanTexture(VkDevice logicalDevice, VkCommandPool pool);
    virtual ~VulkanTexture();

    bool createTexture2D(void* pixels, int width, int height, int channels);
    bool createDepthTexture(int width, int height);

    void destroy();

    VulkanImage* getTextureImage() { return m_textureImage; };
    VkSampler getTextureSampler() { return m_textureSampler; };

private:
    bool createTextureImage(void* pixels, int width, int height, int channels);
    void destroyTextureImage();

    vk::ImageView createTextureImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);
    void destroyTextureImageView();

    bool createTextureSampler();
    void destroyTextureSampler();

    uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
    vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);

    void transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
    vk::CommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(vk::CommandBuffer commandBuffer);

private:
    VkDevice m_logicalDevice;
    VkCommandPool m_commandPool;

    VulkanImage* m_textureImage{};
    VkSampler m_textureSampler;
};

NS_END
