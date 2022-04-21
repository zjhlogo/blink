
/*********************************************************************
 * \file   VulkanResModule.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#include "VulkanResModule.h"
#include "resources/VulkanGeometryLineList.h"
#include "resources/VulkanGeometryTriangleList.h"
#include "resources/VulkanMaterial.h"
#include "resources/VulkanTexture.h"

#include <render_vulkan/VulkanCommandPool.h>
#include <render_vulkan/VulkanDescriptorPool.h>
#include <render_vulkan/VulkanLogicalDevice.h>
#include <render_vulkan/VulkanRenderModule.h>
#include <render_vulkan/VulkanSwapchain.h>

namespace blink
{
    IResModule* getResModule()
    {
        static VulkanResModule s_vulkanResModule;
        return &s_vulkanResModule;
    }

    const tstring VulkanResModule::DEFAULT_TEXTURE = "resource/pink.png";

    bool VulkanResModule::initialize()
    {
        auto vulkanRenderModule = dynamic_cast<VulkanRenderModule*>(getRenderModule());
        m_logicalDevice = &vulkanRenderModule->getLogicalDevice();
        m_swapchain = &vulkanRenderModule->getSwapchain();

        return true;
    }

    void VulkanResModule::terminate()
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
    }

    void VulkanResModule::recreate()
    {
        for (auto kvp : m_materialMap)
        {
            auto material = kvp.second;
            material->recreate();
        }
    }

    ITexture2d* VulkanResModule::createTexture2d(const tstring& filePath)
    {
        auto it = m_texture2dMap.find(filePath);
        if (it != m_texture2dMap.end())
        {
            it->second->incRef();
            return it->second;
        }

        // create new
        auto texture = new VulkanTexture(*m_logicalDevice);
        if (!texture->createTexture2D(filePath))
        {
            SAFE_DELETE(texture);
            return nullptr;
        }

        texture->setId(filePath);
        texture->incRef();
        m_texture2dMap.emplace(std::make_pair(texture->getId(), texture));

        return texture;
    }

    void VulkanResModule::releaseTexture2d(ITexture2d* texture)
    {
        auto it = m_texture2dMap.find(texture->getId());
        if (it != m_texture2dMap.end())
        {
            it->second->decRef();
        }
    }

    IGeometry* VulkanResModule::createGeometry(const tstring& uniqueId, PrimitiveTopology topology)
    {
        auto it = m_geometryMap.find(uniqueId);
        if (it != m_geometryMap.end())
        {
            it->second->incRef();
            return it->second;
        }

        IGeometry* geometry = nullptr;
        if (topology == PrimitiveTopology::TriangleList)
        {
            geometry = new VulkanGeometryTriangleList(*m_logicalDevice);
        }
        else if (topology == PrimitiveTopology::LineList)
        {
            geometry = new VulkanGeometryLineList(*m_logicalDevice);
        }

        geometry->setId(uniqueId);
        geometry->incRef();
        m_geometryMap.emplace(std::make_pair(geometry->getId(), geometry));

        return geometry;
    }

    void VulkanResModule::releaseGeometry(IGeometry* geometry)
    {
        auto it = m_geometryMap.find(geometry->getId());
        if (it != m_geometryMap.end())
        {
            it->second->decRef();
        }
    }

    IMaterial* VulkanResModule::createMaterial(const tstring& filePath)
    {
        // find from map
        auto it = m_materialMap.find(filePath);
        if (it != m_materialMap.end())
        {
            it->second->incRef();
            return it->second;
        }

        // create new
        auto material = new VulkanMaterial(*m_logicalDevice, *m_swapchain);
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

    void VulkanResModule::releaseMaterial(IMaterial* material)
    {
        auto it = m_materialMap.find(material->getId());
        if (it != m_materialMap.end())
        {
            it->second->decRef();
        }
    }
} // namespace blink
