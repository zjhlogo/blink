/*********************************************************************
 * \file   SceneRenderSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   02/07/2022
 *********************************************************************/

#include "SceneRenderSystem.h"

#include <blink/components/Components.h>
#include <blink/type/RenderTypes.h>
#include <core/components/Components.h>
#include <foundation/Log.h>
#include <glm/gtx/quaternion.hpp>
#include <render_vulkan/VulkanCommandBuffer.h>
#include <render_vulkan/VulkanRenderModule.h>
#include <render_vulkan/VulkanUniformBuffer.h>
#include <render_vulkan/resources/VulkanMaterial.h>

#include <map>
#include <unordered_map>

struct InternalRenderData
{
    glm::vec3 pos;
    glm::quat rot;
    blink::PerInstanceUniforms piu;
    blink::IGeometry* geometry;
    blink::uint32 renderLayer;
    int cameraId;
    VkDescriptorBufferInfo piubi;
};

struct RenderDataLight
{
    glm::vec3 pos;
    glm::vec3 color;
    float intensity;
};

SceneRenderSystem::SceneRenderSystem(blink::IApp* app)
    : m_app(app)
{
    //
}

SceneRenderSystem::~SceneRenderSystem()
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

void SceneRenderSystem::render(blink::IRenderData& renderData)
{
    const auto& world = m_app->getEcsWorld().getWorld();

    // collect light data
    RenderDataLight rdLight{};
    {
        static auto lightDataQuery = world.query_builder<const blink::Position, const blink::LightData>().build();
        lightDataQuery.each(
            [&rdLight](flecs::entity e, const blink::Position& pos, const blink::LightData& light)
            {
                rdLight.pos = pos.value;
                rdLight.color = light.color;
                rdLight.intensity = light.intensity;
            });
    }

    // collect camera data into pcus
    std::vector<blink::PerCameraUniforms> pcus;
    {
        static auto cameraDataQuery =
            world.query_builder<const blink::Position, const blink::Rotation, const blink::CameraData>().build();
        cameraDataQuery.each(
            [&pcus,
             &rdLight](flecs::entity e, const blink::Position& pos, const blink::Rotation& rot, const blink::CameraData& camera)
            {
                // setup perframe uniforms
                blink::PerCameraUniforms pcu;

                pcu.lightPos = rdLight.pos;
                pcu.lightColorAndIntensity = glm::vec4(rdLight.color, rdLight.intensity);

                pcu.cameraPos = pos.value;

                pcu.cameraDir = glm::rotate(rot.value, glm::forward());
                auto up = glm::rotate(rot.value, glm::up());
                pcu.matWorldToCamera = glm::lookAt(pcu.cameraPos, pcu.cameraPos + pcu.cameraDir, up);

                pcu.matWorldToCameraInvT = glm::transpose(glm::inverse(glm::mat3(pcu.matWorldToCamera)));
                pcu.matCameraToProjection = camera.matCameraToProjection;
                pcu.matWorldToProjection = pcu.matCameraToProjection * pcu.matWorldToCamera;
                pcus.push_back(pcu);
            });
    }

    // group render object by materials
    std::unordered_map<blink::IMaterial*, std::vector<InternalRenderData>> renderDatas;
    {
        static auto renderObjQuery = world
                                         .query_builder<const blink::Position,
                                                        const blink::Rotation,
                                                        const blink::Renderable,
                                                        const blink::StaticModel>()
                                         .build();
        renderObjQuery.each(
            [&](flecs::entity e,
                const blink::Position& pos,
                const blink::Rotation& rot,
                const blink::Renderable& renderable,
                const blink::StaticModel& model)
            {
                auto material = model.material;
                if (!material) return;

                auto geometry = model.geometry;
                if (!geometry) return;

                blink::PerInstanceUniforms piu;
                piu.matLocalToWorld = glm::translate(glm::identity<glm::mat4>(), pos.value) * glm::mat4_cast(rot.value);
                piu.matLocalToWorldInvT = glm::transpose(glm::inverse(glm::mat3(piu.matLocalToWorld)));

                auto findIt = renderDatas.find(material);
                if (findIt != renderDatas.end())
                {
                    findIt->second.push_back({pos.value, rot.value, piu, geometry, renderable.renderLayer, 0, {}});
                }
                else
                {
                    std::vector<InternalRenderData> dataLists;
                    dataLists.push_back({pos.value, rot.value, piu, geometry, renderable.renderLayer, 0, {}});
                    renderDatas.emplace(material, dataLists);
                }
            });
    }

    // group render features
    std::map<int, blink::RenderFeature> renderFeatures;
    {
        static auto renderFeatureQuery = world.query_builder<const blink::RenderFeature>().build();
        renderFeatureQuery.each(
            [&](flecs::entity e, const blink::RenderFeature& renderFeature)
            {
                auto findIt = renderFeatures.find(renderFeature.order);
                if (findIt != renderFeatures.end())
                {
                    LOGE("duplicate render feature order {0}", renderFeature.order);
                }
                else
                {
                    renderFeatures.emplace(renderFeature.order, renderFeature);
                }
            });
    }

    auto vulkanRenderData = (blink::VulkanRenderData*)&renderData;

    // start rendering

    // for each camera
    int cameraId = 1;
    for (const auto& pcu : pcus)
    {
        // bind per camera uniforms
        VkDescriptorBufferInfo pcubi{};
        vulkanRenderData->pcub->appendData(&pcu, sizeof(pcu), &pcubi);

        // render features
        for (const auto& kvpFeature : renderFeatures)
        {
            const auto& renderFeatureData = kvpFeature.second;

            if (renderFeatureData.overrideMaterial == nullptr)
            {
                // render mesh group by material
                for (auto& kvp : renderDatas)
                {
                    blink::IMaterial* material = kvp.first;
                    auto vulkanMaterial = dynamic_cast<blink::VulkanMaterial*>(material);

                    vulkanMaterial->bindPipeline(*vulkanRenderData->commandBuffer);

                    // upload per material uniform
                    vulkanMaterial->uploadPerMaterialUniforms(*vulkanRenderData->pmub);
                    vulkanMaterial->uploadPerCameraUniforms(pcubi);

                    for (auto& rdo : kvp.second)
                    {
                        if ((rdo.renderLayer & renderFeatureData.renderLayer) == 0) continue;

                        // bind per instance uniform
                        if (rdo.cameraId != cameraId)
                        {
                            rdo.cameraId = cameraId;
                            rdo.piu.matLocalToProjection = pcu.matWorldToProjection * rdo.piu.matLocalToWorld;
                            vulkanRenderData->piub->appendData(&rdo.piu, sizeof(rdo.piu), &rdo.piubi);
                        }
                        vulkanMaterial->uploadPerInstanceUniforms(rdo.piubi);

                        // update vertex input and uniform buffers
                        vulkanMaterial->updateBufferInfos(*vulkanRenderData->commandBuffer,
                                                          (blink::VulkanGeometry*)rdo.geometry);

                        vkCmdDrawIndexed(*vulkanRenderData->commandBuffer, rdo.geometry->getNumIndices(), 1, 0, 0, 0);
                    }
                }
            }
            else
            {
                blink::IMaterial* material = renderFeatureData.overrideMaterial;
                auto vulkanMaterial = dynamic_cast<blink::VulkanMaterial*>(material);

                vulkanMaterial->bindPipeline(*vulkanRenderData->commandBuffer);

                // upload per material uniform
                vulkanMaterial->uploadPerMaterialUniforms(*vulkanRenderData->pmub);
                vulkanMaterial->uploadPerCameraUniforms(pcubi);

                for (auto& kvp : renderDatas)
                {
                    for (auto& rdo : kvp.second)
                    {
                        if ((rdo.renderLayer & renderFeatureData.renderLayer) == 0) continue;

                        // bind per instance uniform
                        if (rdo.cameraId != cameraId)
                        {
                            rdo.cameraId = cameraId;
                            rdo.piu.matLocalToProjection = pcu.matWorldToProjection * rdo.piu.matLocalToWorld;
                            vulkanRenderData->piub->appendData(&rdo.piu, sizeof(rdo.piu), &rdo.piubi);
                        }
                        vulkanMaterial->uploadPerInstanceUniforms(rdo.piubi);

                        // update vertex input and uniform buffers
                        vulkanMaterial->updateBufferInfos(*vulkanRenderData->commandBuffer,
                                                          (blink::VulkanGeometry*)rdo.geometry);

                        vkCmdDrawIndexed(*vulkanRenderData->commandBuffer, rdo.geometry->getNumIndices(), 1, 0, 0, 0);
                    }
                }
            }
        }

        ++cameraId;
    }
}
