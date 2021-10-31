/*!
 * \file RenderUtil.cpp
 *
 * \author zjhlogo
 * \date 2021/10/25
 *
 *
 */
#include "RenderUtil.h"
#include "../geometry/Mesh.h"
#include "../material/Material.h"

#include <render_vulkan/VulkanBuffer.h>
#include <render_vulkan/VulkanCommandBuffer.h>
#include <render_vulkan/VulkanDescriptorSet.h>
#include <render_vulkan/VulkanPipeline.h>

NS_BEGIN

void RenderUtil::drawMesh(VulkanCommandBuffer& commandBuffer, Mesh* mesh, Material* material, const glm::vec3& pos, const glm::quat& rot)
{
    // TODO: update uniform data, texture data
    material->uploadUniformBuffer(pos, rot);

    auto& pipeline = material->getPipeline();

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    // vertex, index binding
    VkDeviceSize offset = 0;
    VkBuffer vertexBuffer = *(mesh->getVertexBuffer());
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &offset);

    VkBuffer indexBuffer = *(mesh->getIndexBuffer());
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    // uniforms, textures binding
    VkDescriptorSet descriptorSet = material->getDescriptorSet();
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);

    // draw
    vkCmdDrawIndexed(commandBuffer, mesh->getNumIndices(), 1, 0, 0, 0);
}

NS_END
