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

#include <vector>

namespace blink
{
    class VulkanImage;
    class VulkanLogicalDevice;
    class VulkanCommandPool;

    class VulkanTexture : public ITexture2d
    {
    public:
        VulkanTexture(VulkanLogicalDevice& logicalDevice);
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

        VulkanImage* m_textureImage{};
        VkSampler m_textureSampler{};
    };

} // namespace blink