/*********************************************************************
 * \file   SceneRenderSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   02/07/2022
 *********************************************************************/

#include "SceneRenderSystem.h"

#include <blink/components/Components.h>
#include <core/components/Components.h>
#include <foundation/Log.h>
#include <glm/gtx/quaternion.hpp>
#include <render_vulkan/VulkanCommandBuffer.h>
#include <render_vulkan/VulkanRenderModule.h>
#include <render_vulkan/VulkanUniformBuffer.h>
#include <render_vulkan/resources/VulkanMaterial.h>

#include <map>
#include <unordered_map>

struct PerCameraUniforms
{
    glm::mat4 matWorldToCamera;
    glm::mat3 matWorldToCameraInvT;
    glm::mat4 matCameraToProjection;
    glm::mat4 matWorldToProjection;
    glm::vec3 cameraPos;
    glm::vec3 cameraDir;
    glm::vec3 lightPos;
    glm::vec4 lightColorAndIntensity;
    int cameraId;
};

struct PerInstanceUniforms
{
    glm::mat4 matLocalToWorld;
    glm::mat3 matLocalToWorldInvT;
    glm::mat4 matLocalToProjection;
};

struct InternalRenderData
{
    glm::vec3 pos;
    glm::quat rot;
    PerInstanceUniforms piu;
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

void uploadPerCameraUniforms(PerCameraUniforms& pcu,
                             VkDescriptorBufferInfo& pcubi,
                             int cameraId,
                             blink::VulkanMaterial* vulkanMaterial,
                             blink::VulkanRenderData* vulkanRenderData)
{
    if (pcu.cameraId == cameraId)
    {
        vulkanMaterial->uploadPerCameraUniforms(pcubi);
        return;
    }

    auto& pipeline = vulkanMaterial->getPipeline();
    auto pcub = pipeline.getPredefineUniformBlock(blink::VulkanPipeline::PredefineUniformBinding::PerCameraUniformBinding);
    if (!pcub) return;

    // upload per camera uniforms
    pcub->prepareBuffer();
    pcub->setUniformMember("matWorldToCamera", pcu.matWorldToCamera);
    pcub->setUniformMember("matWorldToCameraInvT", pcu.matWorldToCameraInvT);
    pcub->setUniformMember("matCameraToProjection", pcu.matCameraToProjection);
    pcub->setUniformMember("matWorldToProjection", pcu.matWorldToProjection);
    pcub->setUniformMember("cameraPos", pcu.cameraPos);
    pcub->setUniformMember("cameraDir", pcu.cameraDir);
    pcub->setUniformMember("lightPos", pcu.lightPos);
    pcub->setUniformMember("lightColorAndIntensity", pcu.lightColorAndIntensity);
    vulkanRenderData->pcub->appendData(pcub->getBufferData(), pcub->getBufferSize(), &pcubi);
    vulkanMaterial->uploadPerCameraUniforms(pcubi);

    pcu.cameraId = cameraId;
}

void uploadPerInstanceUniforms(InternalRenderData& ird,
                               int cameraId,
                               blink::VulkanMaterial* vulkanMaterial,
                               blink::VulkanRenderData* vulkanRenderData)
{
    // bind per instance uniform
    if (ird.cameraId == cameraId)
    {
        vulkanMaterial->uploadPerInstanceUniforms(ird.piubi);
        return;
    }

    auto& pipeline = vulkanMaterial->getPipeline();
    auto piub = pipeline.getPredefineUniformBlock(blink::VulkanPipeline::PredefineUniformBinding::PerInstanceUniformBinding);
    if (!piub) return;

    piub->prepareBuffer();
    piub->setUniformMember("matLocalToWorld", ird.piu.matLocalToWorld);
    piub->setUniformMember("matLocalToWorldInvT", ird.piu.matLocalToWorld);
    piub->setUniformMember("matLocalToProjection", ird.piu.matLocalToProjection);
    vulkanRenderData->piub->appendData(piub->getBufferData(), piub->getBufferSize(), &ird.piubi);

    ird.cameraId = cameraId;
}

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
    std::vector<PerCameraUniforms> pcus;
    {
        static auto cameraDataQuery =
            world.query_builder<const blink::Position, const blink::Rotation, const blink::CameraData>().build();
        cameraDataQuery.each(
            [&pcus,
             &rdLight](flecs::entity e, const blink::Position& pos, const blink::Rotation& rot, const blink::CameraData& camera)
            {
                // setup perframe uniforms
                PerCameraUniforms pcu;

                pcu.lightPos = rdLight.pos;
                pcu.lightColorAndIntensity = glm::vec4(rdLight.color, rdLight.intensity);

                pcu.cameraPos = pos.value;

                pcu.cameraDir = glm::rotate(rot.value, glm::forward());
                auto up = glm::rotate(rot.value, glm::up());
                pcu.matWorldToCamera = glm::lookAt(pcu.cameraPos, pcu.cameraPos + pcu.cameraDir, up);

                pcu.matWorldToCameraInvT = glm::transpose(glm::inverse(glm::mat3(pcu.matWorldToCamera)));
                pcu.matCameraToProjection = camera.matCameraToProjection;
                pcu.matWorldToProjection = pcu.matCameraToProjection * pcu.matWorldToCamera;
                pcu.cameraId = 0;
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

                PerInstanceUniforms piu;
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
    for (auto& pcu : pcus)
    {
        VkDescriptorBufferInfo pcubi{};

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

                    uploadPerCameraUniforms(pcu, pcubi, cameraId, vulkanMaterial, vulkanRenderData);

                    // upload per material uniform
                    vulkanMaterial->uploadPerMaterialUniforms(*vulkanRenderData->pmub);

                    for (auto& ird : kvp.second)
                    {
                        if ((ird.renderLayer & renderFeatureData.renderLayer) == 0) continue;

                        uploadPerInstanceUniforms(ird, cameraId, vulkanMaterial, vulkanRenderData);

                        // update vertex input and uniform buffers
                        vulkanMaterial->updateBufferInfos(*vulkanRenderData->commandBuffer,
                                                          (blink::VulkanGeometry*)ird.geometry);

                        vkCmdDrawIndexed(*vulkanRenderData->commandBuffer, ird.geometry->getNumIndices(), 1, 0, 0, 0);
                    }
                }
            }
            else
            {
                blink::IMaterial* material = renderFeatureData.overrideMaterial;
                auto vulkanMaterial = dynamic_cast<blink::VulkanMaterial*>(material);

                vulkanMaterial->bindPipeline(*vulkanRenderData->commandBuffer);

                uploadPerCameraUniforms(pcu, pcubi, cameraId, vulkanMaterial, vulkanRenderData);

                // upload per material uniform
                vulkanMaterial->uploadPerMaterialUniforms(*vulkanRenderData->pmub);

                // render mesh group by material
                for (auto& kvp : renderDatas)
                {
                    for (auto& ird : kvp.second)
                    {
                        if ((ird.renderLayer & renderFeatureData.renderLayer) == 0) continue;

                        uploadPerInstanceUniforms(ird, cameraId, vulkanMaterial, vulkanRenderData);

                        // update vertex input and uniform buffers
                        vulkanMaterial->updateBufferInfos(*vulkanRenderData->commandBuffer,
                                                          (blink::VulkanGeometry*)ird.geometry);

                        vkCmdDrawIndexed(*vulkanRenderData->commandBuffer, ird.geometry->getNumIndices(), 1, 0, 0, 0);
                    }
                }
            }
        }

        ++cameraId;
    }
}
