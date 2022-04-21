
/*********************************************************************
 * \file   VulkanMaterial.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#include "VulkanMaterial.h"
#include "../VulkanResourceModule.h"
#include "VulkanTexture.h"

#include <core/modules/IResourceModule.h>
#include <core/resources/IGeometry.h>
#include <foundation/File.h>
#include <foundation/Log.h>
#include <render_vulkan/VulkanCommandBuffer.h>
#include <render_vulkan/VulkanImage.h>
#include <render_vulkan/VulkanLogicalDevice.h>
#include <render_vulkan/VulkanPipeline.h>
#include <render_vulkan/VulkanSwapchain.h>
#include <render_vulkan/VulkanUniformBuffer.h>
#include <tinygltf/json.hpp>

namespace blink
{
    VulkanMaterial::VulkanMaterial(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain)
        : m_logicalDevice(logicalDevice)
        , m_swapchain(swapchain)
    {
    }

    VulkanMaterial::~VulkanMaterial()
    {
        //
        destroy();
    }

    void VulkanMaterial::bindPipeline(VulkanCommandBuffer& commandBuffer)
    {
        //
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pipeline);
    }

    bool VulkanMaterial::bindPerMaterialUniforms(VulkanCommandBuffer& commandBuffer,
                                                 VulkanUniformBuffer& pmub,
                                                 VkDescriptorBufferInfo& pmubi)
    {
        pmub.appendData(&m_pmu, sizeof(m_pmu), &pmubi);

        return true;
    }

    bool VulkanMaterial::updateBufferInfos(VulkanCommandBuffer& commandBuffer,
                                           IGeometry* geometry,
                                           const VkDescriptorBufferInfo& pfubi,
                                           const VkDescriptorBufferInfo& pmubi,
                                           const VkDescriptorBufferInfo& piubi)
    {
        auto inputMask = m_pipeline->getVertexInputMask();
        if (!geometry->checkInputMask(inputMask)) return false;
        if (m_topology != geometry->getTopology()) return false;

        if (m_topology == VK_PRIMITIVE_TOPOLOGY_LINE_LIST || m_topology == VK_PRIMITIVE_TOPOLOGY_LINE_STRIP)
        {
            vkCmdSetLineWidth(commandBuffer, 1.0f);
        }

        VkBuffer buffer = *geometry->getVulkanBuffer();

        for (int i = 0; i < VulkanPipeline::MaxInputLocationMaskBit; ++i)
        {
            uint32 currInputMask = (1 << i);
            if ((currInputMask & inputMask) == currInputMask)
            {
                auto offset = geometry->getVertexInputOffset(currInputMask);
                vkCmdBindVertexBuffers(commandBuffer, i, 1, &buffer, &offset);
            }
        }

        vkCmdBindIndexBuffer(commandBuffer, buffer, geometry->getIndicesOffset(), VK_INDEX_TYPE_UINT16);

        return m_pipeline->bindDescriptorSets(commandBuffer, pfubi, pmubi, piubi, m_imageInfos);
    }

    bool VulkanMaterial::create(const tstring& filePath)
    {
        destroy();

        if (!loadConfigFromFile(filePath)) return false;
        m_filePath = filePath;

        loadTextures();

        m_pipeline = new VulkanPipeline(m_logicalDevice, m_swapchain);
        if (!m_pipeline->create(m_vertexShader, m_fragmentShader, m_polygonMode, m_topology))
        {
            return false;
        }

        return true;
    }

    bool VulkanMaterial::recreate()
    {
        if (m_pipeline)
        {
            return m_pipeline->recreate();
        }

        return true;
    }

    void VulkanMaterial::destroy()
    {
        for (auto texture : m_textures)
        {
            SAFE_RELEASE(texture);
        }
        m_textures.clear();
        m_imageInfos.clear();

        SAFE_DELETE(m_pipeline);
        m_filePath.clear();
        m_vertexShader.clear();
        m_fragmentShader.clear();
        m_polygonMode = VK_POLYGON_MODE_FILL;
        m_topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        m_texturePaths.clear();
    }

    bool VulkanMaterial::loadConfigFromFile(const tstring& filePath)
    {
        tstring fileContent;
        if (!File::readFileIntoString(fileContent, filePath))
        {
            LOGE("Open file failed {0}", filePath);
            return false;
        }

        auto j = nlohmann::json::parse(fileContent);

        m_vertexShader = j["vertex_shader"].get<tstring>();
        m_fragmentShader = j["fragment_shader"].get<tstring>();

        // roughness
        {
            auto jroughness = j["roughness"];
            if (!jroughness.is_null())
            {
                m_pmu.roughness = jroughness.get<float>();
            }
        }

        // metallic
        {
            auto jmetallic = j["metallic"];
            if (!jmetallic.is_null())
            {
                m_pmu.metallic = jmetallic.get<float>();
            }
        }

        // color
        {
            auto jcolor = j["color"];
            if (jcolor.is_array() && jcolor.size() == 3)
            {
                m_pmu.color = glm::vec3(jcolor[0].get<float>(), jcolor[1].get<float>(), jcolor[2].get<float>());
            }
        }

        // wireframe
        {
            auto jwireframe = j["wireframe"];
            if (!jwireframe.is_null())
            {
                if (jwireframe.get<bool>())
                {
                    m_polygonMode = VK_POLYGON_MODE_LINE;
                }
            }
        }

        // linelist
        {
            auto jlinelist = j["line_list"];
            if (!jlinelist.is_null())
            {
                if (jlinelist.get<bool>())
                {
                    m_topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
                }
            }
        }

        // textures
        {
            auto jtextures = j["textures"];
            if (jtextures.is_array())
            {
                for (int i = 0; i < jtextures.size(); ++i)
                {
                    m_texturePaths.push_back(jtextures[i].get<tstring>());
                }
            }
        }

        return true;
    }

    bool VulkanMaterial::loadTextures()
    {
        for (const auto& filePath : m_texturePaths)
        {
            auto texture = getResourceModule()->createTexture2d(filePath);
            if (!texture) texture = getResourceModule()->createTexture2d(VulkanResourceModule::DEFAULT_TEXTURE);
            if (!texture) return false;

            m_textures.push_back(texture);

            auto vulkanTexture = dynamic_cast<VulkanTexture*>(texture);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = vulkanTexture->getTextureImage()->getImageView();
            imageInfo.sampler = vulkanTexture->getTextureSampler();
            m_imageInfos.push_back(imageInfo);
        }

        return true;
    }

} // namespace blink
