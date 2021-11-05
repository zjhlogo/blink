/**

    @file      Material.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "Material.h"

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

    Material::~Material() { destroy(); }

    bool Material::create(const tstring& filePath)
    {
        static const std::vector<VkVertexInputBindingDescription> s_bindingDescription = {
            {0, sizeof(glm::vec3), VK_VERTEX_INPUT_RATE_VERTEX}, // position
            {1, sizeof(glm::vec3), VK_VERTEX_INPUT_RATE_VERTEX}, // normal
            {2, sizeof(glm::vec2), VK_VERTEX_INPUT_RATE_VERTEX}, // uv0
        };

        static const std::vector<VkVertexInputAttributeDescription> s_attributeDescriptions = {
            {0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0},
            {1, 1, VK_FORMAT_R32G32B32_SFLOAT, 0},
            {2, 2, VK_FORMAT_R32G32_SFLOAT, 0},
        };

        destroy();

        if (!loadConfigFromFile(filePath)) return false;
        m_filePath = filePath;

        m_pipeline = new VulkanPipeline(m_logicalDevice, m_swapchain, m_descriptorPool);
        if (!m_pipeline->create(s_bindingDescription, s_attributeDescriptions, m_vertexShader, m_fragmentShader, m_wireframe))
        {
            return false;
        }

        return true;
    }

    void Material::destroy()
    {
        SAFE_DELETE(m_pipeline);
        m_filePath.clear();
        m_vertexShader.clear();
        m_fragmentShader.clear();
        m_wireframe = false;
    }

    void Material::bindPipeline(VulkanCommandBuffer& commandBuffer) { vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pipeline); }

    bool Material::bindUniformBuffer(VulkanCommandBuffer& commandBuffer, VulkanUniformBuffer& uniformBuffer, const glm::vec3& pos, const glm::quat& rot)
    {
        glm::mat4 localToWorld = glm::translate(glm::identity<glm::mat4>(), pos) * glm::mat4_cast(rot);

        auto beginOfData = uniformBuffer.getCurrentPos();
        auto dataSize = sizeof(localToWorld);
        if (!uniformBuffer.appendData(&localToWorld, dataSize)) return false;

        const auto& pfuBufferInfo = uniformBuffer.getPerFrameBufferInfo();

        VkDescriptorBufferInfo piuBufferInfo{};
        piuBufferInfo.buffer = uniformBuffer;
        piuBufferInfo.offset = beginOfData;
        piuBufferInfo.range = dataSize;

        auto descriptorSet = m_pipeline->updateDescriptorSet(pfuBufferInfo, piuBufferInfo, m_textures);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->getPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);

        return true;
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

        return true;
    }

} // namespace blink
