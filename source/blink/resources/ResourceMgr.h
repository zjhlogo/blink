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

    class Texture2d;
    class Geometry;
    class Material;
    class IGeometryBuilder;

    class ResourceMgr
    {
    public:
        static const tstring DEFAULT_TEXTURE;

    public:
        static ResourceMgr& getInstance();

        bool initialize(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain);
        void terminate();
        void recreate();

        Texture2d* createTexture2d(const tstring& filePath);
        void releaseTexture2d(Texture2d* texture);

        Geometry* createGeometry(const IGeometryBuilder& builder);
        Geometry* createGeometry(const tstring& filePath);
        void releaseGeometry(Geometry* geometry);

        Material* createMaterial(const tstring& filePath);
        void releaseMaterial(Material* material);

    private:
        ResourceMgr();
        ~ResourceMgr();

    private:
        VulkanLogicalDevice* m_logicalDevice{};
        VulkanSwapchain* m_swapchain{};

        std::unordered_map<tstring, Texture2d*> m_texture2dMap;
        std::unordered_map<tstring, Geometry*> m_geometryMap;
        std::unordered_map<tstring, Material*> m_materialMap;
    };
} // namespace blink