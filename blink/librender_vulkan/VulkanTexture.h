/*!
 * \file VulkanTexture.h
 *
 * \author zjhlogo
 * \date 2020/01/19
 *
 *
 */
#pragma once

#include <Texture.h>
#include <vulkan/vulkan.hpp>

NS_BEGIN

class VulkanTexture : public Texture
{
public:
    VulkanTexture(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vk::Queue graphicsQueue, vk::CommandPool commandPool);
    virtual ~VulkanTexture();

    bool createTexture2D(void* pixels, int width, int height, int channels);
    bool createDepthTexture(int width, int height);

    void destroy();

    vk::Image& getTextureImage() { return m_textureImage; };
    vk::DeviceMemory& getTextureImageMemory() { return m_textureImageMemory; };
    vk::ImageView& getTextureImageView() { return m_textureImageView; };
    vk::Sampler& getTextureSampler() { return m_textureSampler; };

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
    vk::PhysicalDevice m_physicalDevice;
    vk::Device m_logicalDevice;
    vk::Queue m_graphicsQueue;
    vk::CommandPool m_commandPool;

    vk::Image m_textureImage;
    vk::DeviceMemory m_textureImageMemory;

    vk::ImageView m_textureImageView;
    vk::Sampler m_textureSampler;
};

NS_END
