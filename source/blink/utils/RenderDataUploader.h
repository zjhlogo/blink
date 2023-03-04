/*********************************************************************
 * \file   RenderDataUploader.h
 * \brief
 *
 * \author zjhlogo
 * \date   2-12 2023
 *********************************************************************/
#pragma once
#include <core/resources/IGeometry.h>
#include <foundation/BaseTypesGlm.h>
#include <render_vulkan/resources/VulkanMaterial.h>
#include <vulkan/vulkan_core.h>

class RenderDataUploader
{
public:
    struct FrameRenderData
    {
        glm::vec3 lightPos;
        glm::vec3 lightColor;
        float lightIntensity;
        float time;
        glm::vec2 screenResolution;

        bool uploaded;
        VkDescriptorBufferInfo bufferInfo;
    };

    struct CameraRenderData
    {
        glm::mat4 matCameraToProjection;
        glm::mat4 matWorldToProjection;
        glm::quat cameraRot;
        glm::vec3 cameraPos;

        int cameraId;
        VkDescriptorBufferInfo bufferInfo;
    };

    struct EntityRenderData
    {
        glm::vec3 pos;
        glm::quat rot;
        glm::vec3 scale;
        blink::IGeometry* geometry;
        uint32_t renderLayer;
        int cameraId;
    };

    struct MaterialRenderData
    {
        int cameraId;
        VkDescriptorBufferInfo bufferInfo;
        std::vector<EntityRenderData> entityRenderData;
    };

public:
    static void uploadFrameUniforms(FrameRenderData& frameRenderData, blink::VulkanMaterial* material, blink::VulkanUniformBuffer& uniformBuffer);
    static void uploadCameraUniforms(CameraRenderData& cameraRenderData,
                                     int cameraId,
                                     blink::VulkanMaterial* material,
                                     blink::VulkanUniformBuffer& uniformBuffer);
    static void uploadEntityPushConstant(EntityRenderData& entityRenderData,
                                         int cameraId,
                                         blink::VulkanMaterial* material,
                                         blink::VulkanCommandBuffer& commandBuffer);
    static void uploadMaterialUniforms(MaterialRenderData& materialRenderData,
                                       int cameraId,
                                       blink::VulkanMaterial* material,
                                       blink::VulkanUniformBuffer& uniformBuffer);
};
