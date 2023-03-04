/*********************************************************************
 * \file   SceneRenderSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   02/07/2022
 *********************************************************************/

#include "SceneRenderSystem.h"
#include "../components/Components.h"
#include "../utils/RenderDataUploader.h"

#include <core/components/Components.h>
#include <foundation/Log.h>
#include <glm/gtx/quaternion.hpp>
#include <render_vulkan/VulkanCommandBuffer.h>
#include <render_vulkan/VulkanRenderModule.h>
#include <render_vulkan/VulkanUniformBuffer.h>
#include <render_vulkan/resources/VulkanMaterial.h>

#include <map>
#include <unordered_map>

SceneRenderSystem::SceneRenderSystem(blink::IApp* app)
    : m_app(app)
{
    //
}

bool SceneRenderSystem::initialize()
{
    return true;
}

void SceneRenderSystem::terminate()
{
    //
}

void SceneRenderSystem::render()
{
    const auto& world = m_app->getEcsWorld().getWorld();
    auto renderModule = dynamic_cast<blink::VulkanRenderModule*>(blink::getRenderModule());
    const auto& surfaceSize = renderModule->getSurfaceSize();

    // collect light data
    RenderDataUploader::FrameRenderData frameRenderData{};
    {
        frameRenderData.time = std::fmodf(world.time(), 3600.0f);
        frameRenderData.screenResolution = surfaceSize;
        static auto lightDataQuery = world.query_builder<const blink::Position, const blink::LightData>().build();
        lightDataQuery.each([&frameRenderData](flecs::entity e, const blink::Position& pos, const blink::LightData& light) {
            frameRenderData.lightPos = pos.value;
            frameRenderData.lightColor = light.color;
            frameRenderData.lightIntensity = light.intensity;
        });
    }

    // collect camera data into cameraGroups
    std::vector<RenderDataUploader::CameraRenderData> cameraRenderDatas;
    {
        static auto cameraDataQuery = world.query_builder<const blink::Position, const blink::Rotation, const blink::CameraData>().build();
        cameraDataQuery.each([&cameraRenderDatas](flecs::entity e, const blink::Position& pos, const blink::Rotation& rot, const blink::CameraData& camera) {
            // setup perFrame uniforms
            RenderDataUploader::CameraRenderData cameraRenderData{};

            cameraRenderData.cameraPos = pos.value;
            cameraRenderData.cameraRot = rot.value;
            cameraRenderData.matCameraToProjection = camera.matCameraToProjection;
            cameraRenderData.cameraId = 0;

            cameraRenderDatas.push_back(cameraRenderData);
        });
    }

    // group render object by materials
    std::unordered_map<blink::IMaterial*, RenderDataUploader::MaterialRenderData> materialGroups;
    {
        static auto renderObjQuery =
            world.query_builder<const blink::Position, const blink::Rotation, const blink::Scale, const blink::Renderable, const blink::StaticModel>().build();
        renderObjQuery.each([&](flecs::entity e,
                                const blink::Position& pos,
                                const blink::Rotation& rot,
                                const blink::Scale& scale,
                                const blink::Renderable& renderable,
                                const blink::StaticModel& model) {
            auto material = model.material;
            if (!material)
            {
                return;
            }

            auto geometry = model.geometry;
            if (!geometry)
            {
                return;
            }

            auto findIt = materialGroups.find(material);
            if (findIt != materialGroups.end())
            {
                findIt->second.entityRenderData.push_back({pos.value, rot.value, scale.value, geometry, renderable.renderLayer, 0});
            }
            else
            {
                RenderDataUploader::MaterialRenderData materialRenderData{};
                materialRenderData.cameraId = 0;
                materialRenderData.entityRenderData.push_back({pos.value, rot.value, scale.value, geometry, renderable.renderLayer, 0});
                materialGroups.emplace(material, materialRenderData);
            }
        });
    }

    // group render features, must ordered
    std::map<int, blink::RenderFeature> featureGroups;
    {
        static auto renderFeatureQuery = world.query_builder<const blink::RenderFeature>().build();
        renderFeatureQuery.each([&](flecs::entity e, const blink::RenderFeature& renderFeature) {
            auto findIt = featureGroups.find(renderFeature.order);
            if (findIt != featureGroups.end())
            {
                LOGE("duplicate render feature order {0}", renderFeature.order);
            }
            else
            {
                featureGroups.emplace(renderFeature.order, renderFeature);
            }
        });
    }

    // start rendering
    auto& commandBuffer = renderModule->getCommandBuffer();
    auto& perFrameUniformBuffer = renderModule->getPerFrameUniformBuffer();
    auto& perMaterialUniformBuffer = renderModule->getPerMaterialUniformBuffer();

    // for each camera
    int cameraId = 1;
    for (auto& cameraRenderData : cameraRenderDatas)
    {
        // render features
        for (const auto& kvpFeature : featureGroups)
        {
            const auto& renderFeatureData = kvpFeature.second;

            if (renderFeatureData.overrideMaterial == nullptr)
            {
                // render mesh group by material
                for (auto& kvpMaterial : materialGroups)
                {
                    blink::IMaterial* material = kvpMaterial.first;
                    auto& materialRenderData = kvpMaterial.second;

                    auto vulkanMaterial = dynamic_cast<blink::VulkanMaterial*>(material);

                    vulkanMaterial->bindPipeline(commandBuffer);

                    RenderDataUploader::uploadFrameUniforms(frameRenderData, vulkanMaterial, perFrameUniformBuffer);
                    RenderDataUploader::uploadCameraUniforms(cameraRenderData, cameraId, vulkanMaterial, perFrameUniformBuffer);
                    RenderDataUploader::uploadMaterialUniforms(materialRenderData, cameraId, vulkanMaterial, perMaterialUniformBuffer);

                    for (auto& entityRenderData : materialRenderData.entityRenderData)
                    {
                        if ((entityRenderData.renderLayer & renderFeatureData.renderLayer) == 0)
                        {
                            continue;
                        }

                        RenderDataUploader::uploadEntityPushConstant(entityRenderData, cameraId, vulkanMaterial, commandBuffer);

                        // update vertex input and uniform buffers
                        vulkanMaterial->updateBufferInfos(commandBuffer, dynamic_cast<blink::VulkanGeometry*>(entityRenderData.geometry));

                        vkCmdDrawIndexed((VkCommandBuffer)commandBuffer, entityRenderData.geometry->getNumIndices(), 1, 0, 0, 0);
                    }
                }
            }
            else
            {
                blink::IMaterial* material = renderFeatureData.overrideMaterial;
                auto vulkanMaterial = dynamic_cast<blink::VulkanMaterial*>(material);

                vulkanMaterial->bindPipeline(commandBuffer);

                RenderDataUploader::uploadFrameUniforms(frameRenderData, vulkanMaterial, perFrameUniformBuffer);
                RenderDataUploader::uploadCameraUniforms(cameraRenderData, cameraId, vulkanMaterial, perFrameUniformBuffer);
                RenderDataUploader::MaterialRenderData materialRenderData{};
                RenderDataUploader::uploadMaterialUniforms(materialRenderData, cameraId, vulkanMaterial, perMaterialUniformBuffer);

                // render mesh group by material
                for (auto& kvpMaterial : materialGroups)
                {
                    for (auto& entityRenderData : kvpMaterial.second.entityRenderData)
                    {
                        if ((entityRenderData.renderLayer & renderFeatureData.renderLayer) == 0)
                        {
                            continue;
                        }

                        RenderDataUploader::uploadEntityPushConstant(entityRenderData, cameraId, vulkanMaterial, commandBuffer);

                        // update vertex input and uniform buffers
                        vulkanMaterial->updateBufferInfos(commandBuffer, dynamic_cast<blink::VulkanGeometry*>(entityRenderData.geometry));

                        vkCmdDrawIndexed((VkCommandBuffer)commandBuffer, entityRenderData.geometry->getNumIndices(), 1, 0, 0, 0);
                    }
                }
            }
        }

        ++cameraId;
    }
}
