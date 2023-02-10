/*!
 * \file VulkanTexture.h
 *
 * \author zjhlogo
 * \date 2020/01/19
 *
 *
 */
#pragma once

#include <core/resources/ITexture2d.h>
#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.h>

namespace blink
{
    class VulkanImage;
    class VulkanLogicalDevice;
    class VulkanCommandPool;

    class VulkanTexture final : public ITexture2d
    {
    public:
        explicit VulkanTexture(VulkanLogicalDevice& logicalDevice);
        ~VulkanTexture() override;

        VulkanTexture(const VulkanTexture& texture) = delete;
        VulkanTexture(VulkanTexture&& texture) = delete;
        VulkanTexture& operator=(const VulkanTexture& texture) = delete;
        VulkanTexture& operator=(VulkanTexture&& texture) = delete;

        bool createTexture2D(const tstring& texFile);
        bool createTexture2D(const void* pixels, uint32_t width, uint32_t height, int channels);
        bool createDepthTexture(uint32_t width, uint32_t height);

        void destroy();

        VulkanImage* getTextureImage() { return m_textureImage; }
        VkSampler getTextureSampler() { return m_textureSampler; }

    private:
        VulkanImage* createTextureImage(const void* pixels, uint32_t width, uint32_t height, int channels);
        void destroyTextureImage();

        VkSampler createTextureSampler();
        void destroyTextureSampler();

    private:
        VulkanLogicalDevice& m_logicalDevice;

        VulkanImage* m_textureImage{};
        VkSampler m_textureSampler{};
    };
} // namespace blink
