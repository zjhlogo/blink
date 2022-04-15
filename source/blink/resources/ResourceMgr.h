/**

    @file      ResourceMgr.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include "../geometries/IGeometry.h"

#include <foundation/BaseTypes.h>

#include <unordered_map>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanSwapchain;

    class Texture2d;
    class Material;

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

        template <typename T, typename = std::enable_if_t<std::is_base_of<IGeometry, T>::value>>
        T* createGeometry(const tstring& uniqueId)
        {
            auto it = m_geometryMap.find(uniqueId);
            if (it != m_geometryMap.end())
            {
                it->second->incRef();
                return dynamic_cast<T*>(it->second);
            }

            auto geometry = new T(*m_logicalDevice);
            geometry->setId(uniqueId);
            geometry->incRef();
            m_geometryMap.emplace(std::make_pair(geometry->getId(), geometry));

            return geometry;
        }

        void releaseGeometry(IGeometry* geometry);

        Material* createMaterial(const tstring& filePath);
        void releaseMaterial(Material* material);

    private:
        ResourceMgr();
        ~ResourceMgr();

    private:
        VulkanLogicalDevice* m_logicalDevice{};
        VulkanSwapchain* m_swapchain{};

        std::unordered_map<tstring, Texture2d*> m_texture2dMap;
        std::unordered_map<tstring, IGeometry*> m_geometryMap;
        std::unordered_map<tstring, Material*> m_materialMap;
    };

} // namespace blink
