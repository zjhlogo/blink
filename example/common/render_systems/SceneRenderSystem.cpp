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

struct RenderDataGeo
{
    glm::vec3 pos;
    glm::quat rot;
    blink::PerInstanceUniforms piu;
    blink::IGeometry* geometry;
    blink::uint32 renderLayer;
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

    // collect camera data into pfus
    std::vector<blink::PerFrameUniforms> pfus;
    {
        static auto cameraDataQuery =
            world.query_builder<const blink::Position, const blink::Rotation, const blink::CameraData>().build();
        cameraDataQuery.each(
            [&pfus,
             &rdLight](flecs::entity e, const blink::Position& pos, const blink::Rotation& rot, const blink::CameraData& camera)
            {
                // setup perframe uniforms
                blink::PerFrameUniforms pfu;

                pfu.lightPos = rdLight.pos;
                pfu.lightColorAndIntensity = glm::vec4(rdLight.color, rdLight.intensity);

                pfu.cameraPos = pos.value;

                pfu.cameraDir = glm::rotate(rot.value, glm::vec3(0.0f, 0.0f, -1.0f));
                auto up = glm::rotate(rot.value, glm::vec3(0.0f, 1.0f, 0.0f));
                pfu.matWorldToCamera = glm::lookAt(pfu.cameraPos, pfu.cameraPos + pfu.cameraDir, up);

                pfu.matWorldToCameraInvT = glm::transpose(glm::inverse(glm::mat3(pfu.matWorldToCamera)));
                pfu.matCameraToProjection = camera.matCameraToProjection;
                pfu.matWorldToProjection = pfu.matCameraToProjection * pfu.matWorldToCamera;
                pfus.push_back(pfu);
            });
    }

    // group render object by materials
    std::unordered_map<blink::IMaterial*, std::vector<RenderDataGeo>> renderDatas;
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
                    findIt->second.push_back({pos.value, rot.value, piu, geometry, renderable.renderLayer});
                }
                else
                {
                    std::vector<RenderDataGeo> dataLists;
                    dataLists.push_back({pos.value, rot.value, piu, geometry, renderable.renderLayer});
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
    for (const auto& pfu : pfus)
    {
        // bind per camera uniforms
        VkDescriptorBufferInfo pfubi{};
        vulkanRenderData->pfub->appendData(&pfu, sizeof(pfu), &pfubi);

        // render features
        for (const auto& kvpFeature : renderFeatures)
        {
            const auto& renderFeatureData = kvpFeature.second;

            if (renderFeatureData.overrideMaterial == nullptr)
            {
                // render mesh group by material
                for (const auto& kvp : renderDatas)
                {
                    blink::IMaterial* material = kvp.first;
                    auto vulkanMaterial = dynamic_cast<blink::VulkanMaterial*>(material);

                    vulkanMaterial->bindPipeline(*vulkanRenderData->commandBuffer);

                    // bind per material uniform
                    VkDescriptorBufferInfo pmubi{};
                    vulkanMaterial->bindPerMaterialUniforms(*vulkanRenderData->commandBuffer, *vulkanRenderData->pmub, pmubi);

                    for (const auto& rdo : kvp.second)
                    {
                        if ((rdo.renderLayer & renderFeatureData.renderLayer) == 0) continue;

                        // bind per instance uniform
                        VkDescriptorBufferInfo piubi{};
                        vulkanRenderData->piub->appendData(&rdo.piu, sizeof(rdo.piu), &piubi);

                        // update vertex input and uniform buffers
                        vulkanMaterial->updateBufferInfos(*vulkanRenderData->commandBuffer,
                                                          (blink::VulkanGeometry*)rdo.geometry,
                                                          pfubi,
                                                          pmubi,
                                                          piubi);

                        vkCmdDrawIndexed(*vulkanRenderData->commandBuffer, rdo.geometry->getNumIndices(), 1, 0, 0, 0);
                    }
                }
            }
            else
            {
                blink::IMaterial* material = renderFeatureData.overrideMaterial;
                auto vulkanMaterial = dynamic_cast<blink::VulkanMaterial*>(material);

                vulkanMaterial->bindPipeline(*vulkanRenderData->commandBuffer);

                // bind per material uniform
                VkDescriptorBufferInfo pmubi{};
                vulkanMaterial->bindPerMaterialUniforms(*vulkanRenderData->commandBuffer, *vulkanRenderData->pmub, pmubi);

                for (const auto& kvp : renderDatas)
                {
                    for (const auto& rdo : kvp.second)
                    {
                        if ((rdo.renderLayer & renderFeatureData.renderLayer) == 0) continue;

                        // bind per instance uniform
                        VkDescriptorBufferInfo piubi{};
                        vulkanRenderData->piub->appendData(&rdo.piu, sizeof(rdo.piu), &piubi);

                        // update vertex input and uniform buffers
                        vulkanMaterial->updateBufferInfos(*vulkanRenderData->commandBuffer,
                                                          (blink::VulkanGeometry*)rdo.geometry,
                                                          pfubi,
                                                          pmubi,
                                                          piubi);

                        vkCmdDrawIndexed(*vulkanRenderData->commandBuffer, rdo.geometry->getNumIndices(), 1, 0, 0, 0);
                    }
                }
            }
        }
    }
}
