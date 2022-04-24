
/*********************************************************************
 * \file   VulkanMaterial.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#include "VulkanMaterial.h"
#include "../VulkanCommandBuffer.h"
#include "../VulkanImage.h"
#include "../VulkanLogicalDevice.h"
#include "../VulkanResModule.h"
#include "../VulkanSwapchain.h"
#include "../VulkanUniformBuffer.h"
#include "VulkanTexture.h"

#include <core/modules/IResModule.h>
#include <core/resources/IGeometry.h>
#include <foundation/File.h>
#include <foundation/Log.h>
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
                                           VulkanGeometry* geometry,
                                           const std::vector<VulkanPipeline::NamedBufferInfo>& bufferInfos)
    {
        // check validation
        auto pipelineVertexAttrs = m_pipeline->getVertexAttrFlags();
        if (!geometry->hasVertexAttrs(pipelineVertexAttrs)) return false;
        if (m_topology != geometry->getTopology()) return false;

        // set line width when nessary
        if (m_topology == PrimitiveTopology::LineList)
        {
            vkCmdSetLineWidth(commandBuffer, 1.0f);
        }

        // setup vertex buffer
        VkBuffer buffer = *geometry->getVulkanBuffer();
        for (int i = 0; i < VertexAttrs::count_; ++i)
        {
            VertexAttrs::underlying_type currVertexAttr = 1 << i;
            if (pipelineVertexAttrs.contains(currVertexAttr))
            {
                auto offset = geometry->getVertexInputOffset(currVertexAttr);
                vkCmdBindVertexBuffers(commandBuffer, i, 1, &buffer, &offset);
            }
        }

        // setup index buffer
        vkCmdBindIndexBuffer(commandBuffer, buffer, geometry->getIndicesOffset(), VK_INDEX_TYPE_UINT16);

        // bind uniforms
        return m_pipeline->bindDescriptorSets(commandBuffer, bufferInfos, m_textureInfos);
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
        for (auto& texture : m_textureInfos)
        {
            SAFE_RELEASE(texture.texture);
        }
        m_textureInfos.clear();

        SAFE_DELETE(m_pipeline);
        m_filePath.clear();
        m_vertexShader.clear();
        m_fragmentShader.clear();
        m_polygonMode = VK_POLYGON_MODE_FILL;
        m_topology = PrimitiveTopology::TriangleList;
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
                    m_topology = PrimitiveTopology::LineList;
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
                    auto texObj = jtextures[i];
                    m_textureInfos.push_back({texObj["name"].get<tstring>(), texObj["path"].get<tstring>()});
                }
            }
        }

        return true;
    }

    bool VulkanMaterial::loadTextures()
    {
        for (auto& texInfo : m_textureInfos)
        {
            auto resModule = getResModule();
            auto texture = resModule->createTexture2d(texInfo.path);
            if (!texture) texture = resModule->createTexture2d(VulkanResModule::DEFAULT_TEXTURE);
            if (!texture) return false;

            texInfo.texture = texture;

            auto vulkanTexture = dynamic_cast<VulkanTexture*>(texture);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = vulkanTexture->getTextureImage()->getImageView();
            imageInfo.sampler = vulkanTexture->getTextureSampler();

            texInfo.imageInfo = imageInfo;
        }

        return true;
    }

} // namespace blink
