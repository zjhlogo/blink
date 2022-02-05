/**

    @file      Material.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "Material.h"
#include "../geometry/Geometry.h"
#include "../resource/ResourceMgr.h"
#include "../texture/Texture2d.h"
#include "../type/RenderTypes.h"

#include <foundation/File.h>
#include <foundation/Log.h>
#include <render_vulkan/VulkanCommandBuffer.h>
#include <render_vulkan/VulkanImage.h>
#include <render_vulkan/VulkanLogicalDevice.h>
#include <render_vulkan/VulkanPipeline.h>
#include <render_vulkan/VulkanSwapchain.h>
#include <render_vulkan/VulkanTexture.h>
#include <render_vulkan/VulkanUniformBuffer.h>
#include <tinygltf/json.hpp>

namespace blink
{
    Material::Material(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain, VulkanDescriptorPool& descriptorPool)
        : m_logicalDevice(logicalDevice)
        , m_swapchain(swapchain)
        , m_descriptorPool(descriptorPool)
    {
    }

    Material::~Material()
    {
        //
        destroy();
    }

    bool Material::create(const tstring& filePath)
    {
        destroy();

        if (!loadConfigFromFile(filePath)) return false;
        m_filePath = filePath;

        loadTextures();

        m_pipeline = new VulkanPipeline(m_logicalDevice, m_swapchain, m_descriptorPool);
        if (!m_pipeline->create(m_vertexShader, m_fragmentShader, m_wireframe))
        {
            return false;
        }

        return true;
    }

    void Material::destroy()
    {
        for (auto texture : m_textures)
        {
            ResourceMgr::getInstance().releaseTexture2d(texture);
        }
        m_textures.clear();
        m_imageInfos.clear();

        SAFE_DELETE(m_pipeline);
        m_filePath.clear();
        m_vertexShader.clear();
        m_fragmentShader.clear();
        m_wireframe = false;
        m_texturePaths.clear();
    }

    void Material::bindPipeline(VulkanCommandBuffer& commandBuffer)
    {
        //
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pipeline);
    }

    bool Material::bindPerMaterialUniforms(VulkanCommandBuffer& commandBuffer, VulkanUniformBuffer& pmub, VkDescriptorBufferInfo& pmubi)
    {
        // TODO: use config file
        PerMaterialUniforms pmu{VEC4_ONE};
        pmub.appendData(&pmu, sizeof(pmu), &pmubi);

        return true;
    }

    bool Material::updateBufferInfos(VulkanCommandBuffer& commandBuffer,
                                     Geometry* geometry,
                                     const VkDescriptorBufferInfo& pfubi,
                                     const VkDescriptorBufferInfo& pmubi,
                                     const VkDescriptorBufferInfo& piubi)
    {
        auto inputMask = m_pipeline->getVertexInputMask();
        if (!geometry->hasAllVertexInputs(inputMask)) return false;

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

    bool Material::loadConfigFromFile(const tstring& filePath)
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
        m_wireframe = j["wireframe"].get<bool>();

        auto jtextures = j["textures"];
        if (jtextures.is_array())
        {
            for (int i = 0; i < jtextures.size(); ++i)
            {
                m_texturePaths.push_back(jtextures[i].get<tstring>());
            }
        }

        return true;
    }

    bool Material::loadTextures()
    {
        for (const auto& filePath : m_texturePaths)
        {
            auto texture = ResourceMgr::getInstance().createTexture2d(filePath);
            if (!texture) texture = ResourceMgr::getInstance().createTexture2d(ResourceMgr::DEFAULT_TEXTURE);
            if (!texture) return false;

            m_textures.push_back(texture);

            auto vulkanTexture = texture->getVulkanTexture();

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = vulkanTexture->getTextureImage()->getImageView();
            imageInfo.sampler = vulkanTexture->getTextureSampler();
            m_imageInfos.push_back(imageInfo);
        }

        return true;
    }

} // namespace blink
