/*********************************************************************
 * \file   VulkanResourceModule.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#include "VulkanResourceModule.h"
#include "resources/VulkanGeometryLineList.h"
#include "resources/VulkanGeometryTriangleList.h"
#include "resources/VulkanMaterial.h"
#include "resources/VulkanTexture.h"
#include "core/modules/IResourceModule.h"

#include <foundation/PathParser.h>
#include <render_vulkan/VulkanCommandPool.h>
#include <render_vulkan/VulkanLogicalDevice.h>
#include <render_vulkan/VulkanRenderModule.h>

namespace blink
{
    IResourceModule* getResourceModule()
    {
        static VulkanResourceModule vulkanResourceModule;
        return &vulkanResourceModule;
    }

    const tstring VulkanResourceModule::DEFAULT_TEXTURE = "/textures/pink.png";

    bool VulkanResourceModule::initialize()
    {
        auto vulkanRenderModule = dynamic_cast<VulkanRenderModule*>(getRenderModule());
        m_logicalDevice = &vulkanRenderModule->getLogicalDevice();
        m_renderPass = &vulkanRenderModule->getRenderPass();

        return true;
    }

    void VulkanResourceModule::terminate()
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
    }

    ITexture2d* VulkanResourceModule::createTexture2d(const tstring& filePath)
    {
        auto formattedPath = PathParser::formatPath(filePath);

        auto it = m_texture2dMap.find(formattedPath);
        if (it != m_texture2dMap.end())
        {
            it->second->incRef();
            return it->second;
        }

        // create new
        auto texture = new VulkanTexture(*m_logicalDevice);
        if (!texture->createTexture2D(formattedPath))
        {
            SAFE_DELETE(texture);
            return nullptr;
        }

        texture->setId(filePath);
        texture->incRef();
        m_texture2dMap.emplace(std::make_pair(texture->getId(), texture));

        return texture;
    }

    void VulkanResourceModule::releaseTexture2d(ITexture2d* texture)
    {
        auto it = m_texture2dMap.find(texture->getId());
        if (it != m_texture2dMap.end())
        {
            it->second->decRef();
        }
    }

    IGeometry* VulkanResourceModule::createGeometry(const tstring& uniqueId, PrimitiveTopology topology)
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
        else
        {
            LOGE("Unsupported topology of {}", topology);
            return nullptr;
        }

        geometry->setId(uniqueId);
        geometry->incRef();
        m_geometryMap.emplace(std::make_pair(geometry->getId(), geometry));

        return geometry;
    }

    void VulkanResourceModule::releaseGeometry(IGeometry* geometry)
    {
        auto it = m_geometryMap.find(geometry->getId());
        if (it != m_geometryMap.end())
        {
            it->second->decRef();
        }
    }

    IMaterial* VulkanResourceModule::createMaterial(const tstring& filePath)
    {
        auto formattedPath = PathParser::formatPath(filePath);

        // find from map
        auto it = m_materialMap.find(formattedPath);
        if (it != m_materialMap.end())
        {
            it->second->incRef();
            return it->second;
        }

        // create new
        auto material = new VulkanMaterial(*m_logicalDevice, *m_renderPass);
        if (!material->create(formattedPath))
        {
            SAFE_DELETE(material);
            return nullptr;
        }

        material->setId(formattedPath);
        material->incRef();
        m_materialMap.emplace(std::make_pair(material->getId(), material));

        return material;
    }

    void VulkanResourceModule::releaseMaterial(IMaterial* material)
    {
        auto it = m_materialMap.find(material->getId());
        if (it != m_materialMap.end())
        {
            it->second->decRef();
        }
    }
} // namespace blink
