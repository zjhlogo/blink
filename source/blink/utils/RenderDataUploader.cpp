/*********************************************************************
 * \file   RenderDataUploader.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   2-12 2023
 *********************************************************************/
#include "RenderDataUploader.h"

#include <glm/gtx/quaternion.hpp>
#include <render_vulkan/VulkanCommandBuffer.h>
#include <render_vulkan/VulkanPipeline.h>
#include <render_vulkan/VulkanUniformBuffer.h>

void RenderDataUploader::uploadFrameUniforms(FrameRenderData& frameRenderData, blink::VulkanMaterial* material, blink::VulkanUniformBuffer& uniformBuffer)
{
    if (frameRenderData.uploaded)
    {
        material->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Frame, frameRenderData.bufferInfo);
        return;
    }

    auto& pipeline = material->getPipeline();
    auto uniformBlock = pipeline.getUniformBlock(blink::UniformBinding::Frame);
    if (!uniformBlock) return;

    // upload per frame uniforms
    uniformBlock->prepareBuffer();
    uniformBlock->setUniformMember("lightPos", &frameRenderData.lightPos);
    uniformBlock->setUniformMember("lightColor", &frameRenderData.lightColor);
    uniformBlock->setUniformMember("screenResolution", &frameRenderData.screenResolution);
    uniformBlock->setUniformMember("time", &frameRenderData.time);
    uniformBlock->setUniformMember("lightIntensity", &frameRenderData.lightIntensity);
    uniformBuffer.appendData(uniformBlock->getBufferData(), uniformBlock->getBufferSize(), &frameRenderData.bufferInfo);
    material->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Frame, frameRenderData.bufferInfo);

    frameRenderData.uploaded = true;
}

void RenderDataUploader::uploadCameraUniforms(CameraRenderData& cameraRenderData,
                                              int cameraId,
                                              blink::VulkanMaterial* material,
                                              blink::VulkanUniformBuffer& uniformBuffer)
{
    if (cameraRenderData.cameraId == cameraId)
    {
        material->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Camera, cameraRenderData.bufferInfo);
        return;
    }

    auto& pipeline = material->getPipeline();
    auto uniformBlock = pipeline.getUniformBlock(blink::UniformBinding::Camera);
    if (!uniformBlock) return;

    auto cameraDir = glm::rotate(cameraRenderData.cameraRot, glm::forward());
    auto up = glm::rotate(cameraRenderData.cameraRot, glm::up());
    auto matWorldToCamera = glm::lookAt(cameraRenderData.cameraPos, cameraRenderData.cameraPos + cameraDir, up);
    auto matWorldToCameraInvT = glm::transpose(glm::inverse(glm::mat3(matWorldToCamera)));
    cameraRenderData.matWorldToProjection = cameraRenderData.matCameraToProjection * matWorldToCamera;

    // upload per camera uniforms
    uniformBlock->prepareBuffer();
    uniformBlock->setUniformMember("matWorldToCamera", &matWorldToCamera);
    uniformBlock->setUniformMember("matCameraToProjection", &cameraRenderData.matCameraToProjection);
    uniformBlock->setUniformMember("matWorldToProjection", &cameraRenderData.matWorldToProjection);
    uniformBlock->setUniformMember("matWorldToCameraInvT", &matWorldToCameraInvT);
    uniformBlock->setUniformMember("cameraPos", &cameraRenderData.cameraPos);
    uniformBlock->setUniformMember("cameraDir", &cameraDir);
    uniformBuffer.appendData(uniformBlock->getBufferData(), uniformBlock->getBufferSize(), &cameraRenderData.bufferInfo);
    material->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Camera, cameraRenderData.bufferInfo);

    cameraRenderData.cameraId = cameraId;
}

void RenderDataUploader::uploadEntityPushConstant(EntityRenderData& entityRenderData,
                                                  int cameraId,
                                                  blink::VulkanMaterial* material,
                                                  blink::VulkanCommandBuffer& commandBuffer)
{
    // bind per instance uniform
    if (entityRenderData.cameraId == cameraId)
    {
        return;
    }

    auto& pipeline = material->getPipeline();

    blink::VulkanPipeline::PushConstantData pushConstantData;
    pushConstantData.localToWorld = glm::translate(glm::identity<glm::mat4>(), entityRenderData.pos) * glm::mat4_cast(entityRenderData.rot);
    pushConstantData.localToWorldInvT = glm::transpose(glm::inverse(glm::mat3(pushConstantData.localToWorld)));

    vkCmdPushConstants(commandBuffer,
                       pipeline.getPipelineLayout(),
                       VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                       0,
                       sizeof(pushConstantData),
                       &pushConstantData);

    entityRenderData.cameraId = cameraId;
}

void RenderDataUploader::uploadMaterialUniforms(MaterialRenderData& materialRenderData,
                                                int cameraId,
                                                blink::VulkanMaterial* material,
                                                blink::VulkanUniformBuffer& uniformBuffer)
{
    // bind per material uniform
    if (materialRenderData.cameraId == cameraId)
    {
        material->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Material, materialRenderData.bufferInfo);
        return;
    }

    auto& pipeline = material->getPipeline();
    auto uniformBlock = pipeline.getUniformBlock(blink::UniformBinding::Material);
    if (!uniformBlock) return;

    if (uniformBlock->getBufferData() != nullptr)
    {
        uniformBuffer.appendData(uniformBlock->getBufferData(), uniformBlock->getBufferSize(), &materialRenderData.bufferInfo);
        material->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Material, materialRenderData.bufferInfo);
    }

    materialRenderData.cameraId = cameraId;
}