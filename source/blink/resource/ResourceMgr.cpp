/**

    @file      ResourceMgr.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/

#include "ResourceMgr.h"
#include "../geometry/Geometry.h"
#include "../geometry/builder/IGeometryBuilder.h"
#include "../geometry/builder/MeshBuilder.h"
#include "../material/Material.h"
#include "../texture/Texture2d.h"

#include <render_vulkan/VulkanCommandPool.h>
#include <render_vulkan/VulkanDescriptorPool.h>
#include <render_vulkan/VulkanLogicalDevice.h>
#include <render_vulkan/VulkanSwapchain.h>

namespace blink
{
    const tstring ResourceMgr::DEFAULT_TEXTURE = "resource/pink.png";

    ResourceMgr& ResourceMgr::getInstance()
    {
        static ResourceMgr s_resourceMgr;
        return s_resourceMgr;
    }

    bool ResourceMgr::initialize(VulkanLogicalDevice& logicalDevice,
                                 VulkanSwapchain& swapchain,
                                 VulkanDescriptorPool& descriptorPool,
                                 VulkanCommandPool& commandPool)
    {
        m_logicalDevice = &logicalDevice;
        m_swapchain = &swapchain;
        m_descriptorPool = &descriptorPool;
        m_commandPool = &commandPool;

        return true;
    }

    void ResourceMgr::terminate()
    {
        for (auto kvp : m_materialMap)
        {
            SAFE_DELETE(kvp.second);
        }
        m_materialMap.clear();

        for (auto kvp : m_geometryMap)
        {
            SAFE_DELETE(kvp.second);
        }
        m_geometryMap.clear();

        for (auto kvp : m_texture2dMap)
        {
            SAFE_DELETE(kvp.second);
        }
        m_texture2dMap.clear();

        m_logicalDevice = nullptr;
        m_swapchain = nullptr;
        m_descriptorPool = nullptr;
        m_commandPool = nullptr;
    }

    Texture2d* ResourceMgr::createTexture2d(const tstring& filePath)
    {
        auto it = m_texture2dMap.find(filePath);
        if (it != m_texture2dMap.end())
        {
            it->second->incRef();
            return it->second;
        }

        // create new
        auto texture = new Texture2d();
        if (!texture->create(*m_logicalDevice, *m_commandPool, filePath))
        {
            SAFE_DELETE(texture);
            return nullptr;
        }

        texture->setId(filePath);
        texture->incRef();
        m_texture2dMap.emplace(std::make_pair(texture->getId(), texture));

        return texture;
    }

    void ResourceMgr::releaseTexture2d(Texture2d* texture)
    {
        auto it = m_texture2dMap.find(texture->getId());
        if (it != m_texture2dMap.end())
        {
            it->second->decRef();
        }
    }

    Geometry* ResourceMgr::createGeometry(const IGeometryBuilder& builder)
    {
        auto uniqueId = builder.getUniqueId();

        auto it = m_geometryMap.find(uniqueId);
        if (it != m_geometryMap.end())
        {
            it->second->incRef();
            return it->second;
        }

        // create new
        std::vector<glm::vec3> vertsPos;
        std::vector<glm::vec3> vertsNormal;
        std::vector<glm::vec2> vertsUv0;
        std::vector<uint16> indices;
        if (!builder.build(vertsPos, indices, &vertsNormal, &vertsUv0))
        {
            return nullptr;
        }

        auto geometry = new Geometry(*m_logicalDevice, *m_commandPool);
        if (!geometry->uploadData(indices, vertsPos, vertsNormal, vertsUv0))
        {
            SAFE_DELETE(geometry);
            return nullptr;
        }

        geometry->setId(uniqueId);
        geometry->incRef();
        m_geometryMap.emplace(std::make_pair(geometry->getId(), geometry));

        return geometry;
    }

    Geometry* ResourceMgr::createGeometry(const tstring& filePath)
    {
        MeshBuilder builder;
        builder.filePath(filePath);

        auto uniqueId = builder.getUniqueId();

        auto it = m_geometryMap.find(uniqueId);
        if (it != m_geometryMap.end())
        {
            it->second->incRef();
            return it->second;
        }

        // create new
        auto geometry = new Geometry(*m_logicalDevice, *m_commandPool);
        if (!builder.build(geometry))
        {
            SAFE_DELETE(geometry);
            return nullptr;
        }

        geometry->setId(uniqueId);
        geometry->incRef();
        m_geometryMap.emplace(std::make_pair(geometry->getId(), geometry));

        return geometry;
    }

    void ResourceMgr::releaseGeometry(Geometry* geometry)
    {
        auto it = m_geometryMap.find(geometry->getId());
        if (it != m_geometryMap.end())
        {
            it->second->decRef();
        }
    }

    Material* ResourceMgr::createMaterial(const tstring& filePath)
    {
        // find from map
        auto it = m_materialMap.find(filePath);
        if (it != m_materialMap.end())
        {
            it->second->incRef();
            return it->second;
        }

        // create new
        auto material = new Material(*m_logicalDevice, *m_swapchain, *m_descriptorPool);
        if (!material->create(filePath))
        {
            SAFE_DELETE(material);
            return nullptr;
        }

        material->setId(filePath);
        material->incRef();
        m_materialMap.emplace(std::make_pair(material->getId(), material));

        return material;
    }

    void ResourceMgr::releaseMaterial(Material* material)
    {
        auto it = m_materialMap.find(material->getId());
        if (it != m_materialMap.end())
        {
            it->second->decRef();
        }
    }
} // namespace blink
