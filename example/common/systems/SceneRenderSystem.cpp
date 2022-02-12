/*********************************************************************
 * \file   SceneRenderSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   02/07/2022
 *********************************************************************/

#include "SceneRenderSystem.h"

#include <blink/component/Components.h>
#include <blink/geometry/Geometry.h>
#include <blink/material/Material.h>
#include <blink/type/RenderTypes.h>
#include <foundation/Log.h>
#include <glm/gtx/quaternion.hpp>
#include <render_vulkan/VulkanCommandBuffer.h>
#include <render_vulkan/VulkanUniformBuffer.h>

#include <map>
#include <unordered_map>

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

void SceneRenderSystem::render(blink::VulkanCommandBuffer& commandBuffer,
                               blink::VulkanUniformBuffer& pfub,
                               blink::VulkanUniformBuffer& pmub,
                               blink::VulkanUniformBuffer& piub)
{
    const auto& world = m_app->getWorld();

    // collect light data
    blink::RenderDataLight rdLight{};
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
        static auto cameraDataQuery = world.query_builder<const blink::Position, const blink::Rotation, const blink::CameraData>().build();
        cameraDataQuery.each(
            [&pfus, &rdLight](flecs::entity e, const blink::Position& pos, const blink::Rotation& rot, const blink::CameraData& camera)
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
                pfu.matCameraToProjection = glm::perspective(camera.fov, camera.aspect, camera.near, camera.far);
                pfu.matWorldToProjection = pfu.matCameraToProjection * pfu.matWorldToCamera;
                pfus.push_back(pfu);
            });
    }

    // group render object by materials
    std::unordered_map<blink::Material*, std::vector<blink::RenderDataGeo>> renderDatas;
    {
        static auto staticModelQuery = world.query_builder<const blink::Position, const blink::Rotation, const blink::StaticModel>().build();
        staticModelQuery.each(
            [&](flecs::entity e, const blink::Position& pos, const blink::Rotation& rot, const blink::StaticModel& model)
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
                    findIt->second.push_back({pos.value, rot.value, piu, geometry});
                }
                else
                {
                    std::vector<blink::RenderDataGeo> dataLists;
                    dataLists.push_back({pos.value, rot.value, piu, geometry});
                    renderDatas.emplace(material, dataLists);
                }
            });
    }

    // group render features
    std::map<int, blink::RenderFeatureData> renderFeatureDatas;
    {
        static auto renderFeatureQuery = world.query_builder<const blink::RenderFeature>().build();
        renderFeatureQuery.each(
            [&](flecs::entity e, const blink::RenderFeature& feature)
            {
                auto findIt = renderFeatureDatas.find(feature.order);
                if (findIt != renderFeatureDatas.end())
                {
                    LOGE("duplicate render feature order {0} <-> {1}", findIt->second.material->getId(), feature.material->getId());
                }
                else
                {
                    auto material = feature.material;
                    if (!material) return;

                    blink::RenderFeatureData renderFeatureData{feature.order, material};
                    renderFeatureDatas.emplace(feature.order, renderFeatureData);
                }
            });
    }

    // start rendering
    for (const auto& pfu : pfus)
    {
        // bind per camera uniforms
        VkDescriptorBufferInfo pfubi{};
        pfub.appendData(&pfu, sizeof(pfu), &pfubi);

        // render mesh group by material
        for (const auto& kvp : renderDatas)
        {
            blink::Material* material = kvp.first;

            material->bindPipeline(commandBuffer);

            // bind per material uniform
            VkDescriptorBufferInfo pmubi{};
            material->bindPerMaterialUniforms(commandBuffer, pmub, pmubi);

            for (const auto& renderData : kvp.second)
            {
                // bind per instance uniform
                VkDescriptorBufferInfo piubi{};
                piub.appendData(&renderData.piu, sizeof(renderData.piu), &piubi);

                // update vertex input and uniform buffers
                material->updateBufferInfos(commandBuffer, renderData.geometry, pfubi, pmubi, piubi);

                vkCmdDrawIndexed(commandBuffer, renderData.geometry->getNumIndices(), 1, 0, 0, 0);
            }
        }

        // render features
        for (const auto& kvpFeature : renderFeatureDatas)
        {
            blink::Material* material = kvpFeature.second.material;
            material->bindPipeline(commandBuffer);

            // bind per material uniform
            VkDescriptorBufferInfo pmubi{};
            material->bindPerMaterialUniforms(commandBuffer, pmub, pmubi);

            for (const auto& kvp : renderDatas)
            {
                for (const auto& renderData : kvp.second)
                {
                    // bind per instance uniform
                    VkDescriptorBufferInfo piubi{};
                    piub.appendData(&renderData.piu, sizeof(renderData.piu), &piubi);

                    // update vertex input and uniform buffers
                    material->updateBufferInfos(commandBuffer, renderData.geometry, pfubi, pmubi, piubi);

                    vkCmdDrawIndexed(commandBuffer, renderData.geometry->getNumIndices(), 1, 0, 0, 0);
                }
            }
        }
    }
}
