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

struct CameraRenderData
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

struct EntityRenderData
{
    glm::vec3 pos;
    glm::quat rot;
    blink::IGeometry* geometry;
    blink::uint32 renderLayer;
    int cameraId;
    VkDescriptorBufferInfo piubi;
};

struct MaterialRenderData
{
    int cameraId;
    VkDescriptorBufferInfo pmubi;
    std::vector<EntityRenderData> entityRenderDatas;
};

struct LightRenderData
{
    glm::vec3 pos;
    glm::vec3 color;
    float intensity;
};

void uploadCameraUniforms(CameraRenderData& crd,
                          VkDescriptorBufferInfo& pcubi,
                          int cameraId,
                          blink::VulkanMaterial* vulkanMaterial,
                          blink::VulkanRenderData* vulkanRenderData)
{
    if (crd.cameraId == cameraId)
    {
        vulkanMaterial->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Camera, pcubi);
        return;
    }

    auto& pipeline = vulkanMaterial->getPipeline();
    auto pcub = pipeline.getUniformBlock(blink::UniformBinding::Camera);
    if (!pcub) return;

    // upload per camera uniforms
    pcub->prepareBuffer();
    pcub->setUniformMember("matWorldToCamera", crd.matWorldToCamera);
    pcub->setUniformMember("matWorldToCameraInvT", crd.matWorldToCameraInvT);
    pcub->setUniformMember("matCameraToProjection", crd.matCameraToProjection);
    pcub->setUniformMember("matWorldToProjection", crd.matWorldToProjection);
    pcub->setUniformMember("cameraPos", crd.cameraPos);
    pcub->setUniformMember("cameraDir", crd.cameraDir);
    pcub->setUniformMember("lightPos", crd.lightPos);
    pcub->setUniformMember("lightColorAndIntensity", crd.lightColorAndIntensity);
    vulkanRenderData->pcub->appendData(pcub->getBufferData(), pcub->getBufferSize(), &pcubi);
    vulkanMaterial->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Camera, pcubi);

    crd.cameraId = cameraId;
}

void uploadInstanceUniforms(EntityRenderData& erd,
                            const CameraRenderData& pcu,
                            int cameraId,
                            blink::VulkanMaterial* vulkanMaterial,
                            blink::VulkanRenderData* vulkanRenderData)
{
    // bind per instance uniform
    if (erd.cameraId == cameraId)
    {
        vulkanMaterial->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Instance, erd.piubi);
        return;
    }

    auto& pipeline = vulkanMaterial->getPipeline();
    auto piub = pipeline.getUniformBlock(blink::UniformBinding::Instance);
    if (!piub) return;

    auto matLocalToWorld = glm::translate(glm::identity<glm::mat4>(), erd.pos) * glm::mat4_cast(erd.rot);
    auto matLocalToWorldInvT = glm::transpose(glm::inverse(glm::mat3(matLocalToWorld)));
    auto matLocalToProjection = pcu.matWorldToProjection * matLocalToWorld;

    piub->prepareBuffer();
    piub->setUniformMember("matLocalToWorld", matLocalToWorld);
    piub->setUniformMember("matLocalToWorldInvT", matLocalToWorldInvT);
    piub->setUniformMember("matLocalToProjection", matLocalToProjection);
    vulkanRenderData->piub->appendData(piub->getBufferData(), piub->getBufferSize(), &erd.piubi);
    vulkanMaterial->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Instance, erd.piubi);

    erd.cameraId = cameraId;
}

void uploadMaterialUniforms(MaterialRenderData& mrd,
                            int cameraId,
                            blink::VulkanMaterial* vulkanMaterial,
                            blink::VulkanRenderData* vulkanRenderData)
{
    // bind per material uniform
    if (mrd.cameraId == cameraId)
    {
        vulkanMaterial->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Material, mrd.pmubi);
        return;
    }

    auto& pipeline = vulkanMaterial->getPipeline();
    auto pmub = pipeline.getUniformBlock(blink::UniformBinding::Material);
    if (!pmub) return;

    vulkanRenderData->piub->appendData(pmub->getBufferData(), pmub->getBufferSize(), &mrd.pmubi);
    vulkanMaterial->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Material, mrd.pmubi);

    mrd.cameraId = cameraId;
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
    LightRenderData lightGroups{};
    {
        static auto lightDataQuery = world.query_builder<const blink::Position, const blink::LightData>().build();
        lightDataQuery.each(
            [&lightGroups](flecs::entity e, const blink::Position& pos, const blink::LightData& light)
            {
                lightGroups.pos = pos.value;
                lightGroups.color = light.color;
                lightGroups.intensity = light.intensity;
            });
    }

    // collect camera data into cameraGroups
    std::vector<CameraRenderData> cameraGroups;
    {
        static auto cameraDataQuery =
            world.query_builder<const blink::Position, const blink::Rotation, const blink::CameraData>().build();
        cameraDataQuery.each(
            [&cameraGroups, &lightGroups](flecs::entity e,
                                          const blink::Position& pos,
                                          const blink::Rotation& rot,
                                          const blink::CameraData& camera)
            {
                // setup perframe uniforms
                CameraRenderData pcu;

                pcu.lightPos = lightGroups.pos;
                pcu.lightColorAndIntensity = glm::vec4(lightGroups.color, lightGroups.intensity);

                pcu.cameraPos = pos.value;

                pcu.cameraDir = glm::rotate(rot.value, glm::forward());
                auto up = glm::rotate(rot.value, glm::up());
                pcu.matWorldToCamera = glm::lookAt(pcu.cameraPos, pcu.cameraPos + pcu.cameraDir, up);

                pcu.matWorldToCameraInvT = glm::transpose(glm::inverse(glm::mat3(pcu.matWorldToCamera)));
                pcu.matCameraToProjection = camera.matCameraToProjection;
                pcu.matWorldToProjection = pcu.matCameraToProjection * pcu.matWorldToCamera;
                pcu.cameraId = 0;
                cameraGroups.push_back(pcu);
            });
    }

    // group render object by materials
    std::unordered_map<blink::IMaterial*, MaterialRenderData> materialGroups;
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

                auto findIt = materialGroups.find(material);
                if (findIt != materialGroups.end())
                {
                    findIt->second.entityRenderDatas.push_back(
                        {pos.value, rot.value, geometry, renderable.renderLayer, 0, {}});
                }
                else
                {
                    MaterialRenderData mrd;
                    mrd.cameraId = 0;
                    mrd.entityRenderDatas.push_back({pos.value, rot.value, geometry, renderable.renderLayer, 0, {}});
                    materialGroups.emplace(material, mrd);
                }
            });
    }

    // group render features
    std::map<int, blink::RenderFeature> featureGroups;
    {
        static auto renderFeatureQuery = world.query_builder<const blink::RenderFeature>().build();
        renderFeatureQuery.each(
            [&](flecs::entity e, const blink::RenderFeature& renderFeature)
            {
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

    auto vulkanRenderData = (blink::VulkanRenderData*)&renderData;

    // start rendering

    // for each camera
    int cameraId = 1;
    for (auto& crd : cameraGroups)
    {
        VkDescriptorBufferInfo pcubi{};

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
                    auto& mrd = kvpMaterial.second;

                    auto vulkanMaterial = dynamic_cast<blink::VulkanMaterial*>(material);

                    vulkanMaterial->bindPipeline(*vulkanRenderData->commandBuffer);

                    uploadCameraUniforms(crd, pcubi, cameraId, vulkanMaterial, vulkanRenderData);
                    uploadMaterialUniforms(mrd, cameraId, vulkanMaterial, vulkanRenderData);

                    for (auto& erd : mrd.entityRenderDatas)
                    {
                        if ((erd.renderLayer & renderFeatureData.renderLayer) == 0) continue;

                        uploadInstanceUniforms(erd, crd, cameraId, vulkanMaterial, vulkanRenderData);

                        // update vertex input and uniform buffers
                        vulkanMaterial->updateBufferInfos(*vulkanRenderData->commandBuffer,
                                                          (blink::VulkanGeometry*)erd.geometry);

                        vkCmdDrawIndexed(*vulkanRenderData->commandBuffer, erd.geometry->getNumIndices(), 1, 0, 0, 0);
                    }
                }
            }
            else
            {
                blink::IMaterial* material = renderFeatureData.overrideMaterial;
                auto vulkanMaterial = dynamic_cast<blink::VulkanMaterial*>(material);

                vulkanMaterial->bindPipeline(*vulkanRenderData->commandBuffer);

                uploadCameraUniforms(crd, pcubi, cameraId, vulkanMaterial, vulkanRenderData);

                MaterialRenderData mrd{};
                uploadMaterialUniforms(mrd, cameraId, vulkanMaterial, vulkanRenderData);

                // render mesh group by material
                for (auto& kvpMaterial : materialGroups)
                {
                    for (auto& erd : kvpMaterial.second.entityRenderDatas)
                    {
                        if ((erd.renderLayer & renderFeatureData.renderLayer) == 0) continue;

                        uploadInstanceUniforms(erd, crd, cameraId, vulkanMaterial, vulkanRenderData);

                        // update vertex input and uniform buffers
                        vulkanMaterial->updateBufferInfos(*vulkanRenderData->commandBuffer,
                                                          (blink::VulkanGeometry*)erd.geometry);

                        vkCmdDrawIndexed(*vulkanRenderData->commandBuffer, erd.geometry->getNumIndices(), 1, 0, 0, 0);
                    }
                }
            }
        }

        ++cameraId;
    }
}
