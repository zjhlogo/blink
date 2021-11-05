/**

    @file      ResourceMgr.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <foundation/BaseTypes.h>

#include <unordered_map>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanSwapchain;
    class VulkanDescriptorPool;
    class VulkanCommandPool;

    class Texture2d;
    class Material;

    class ResourceMgr
    {
    public:
        static ResourceMgr& getInstance();

        bool initialize(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain, VulkanDescriptorPool& descriptorPool, VulkanCommandPool& commandPool);
        void terminate();

        Texture2d* createTexture2d(const tstring& filePath);
        void releaseTexture2d(Texture2d* texture);

        Material* createMaterial(const tstring& filePath);
        void releaseMaterial(Material* material);

    private:
        ResourceMgr(){};
        ~ResourceMgr(){};

    private:
        VulkanLogicalDevice* m_logicalDevice{};
        VulkanSwapchain* m_swapchain{};
        VulkanDescriptorPool* m_descriptorPool{};
        VulkanCommandPool* m_commandPool{};

        std::unordered_map<tstring, Texture2d*> m_texture2dMap;
        std::unordered_map<tstring, Material*> m_materialMap;
    };
} // namespace blink