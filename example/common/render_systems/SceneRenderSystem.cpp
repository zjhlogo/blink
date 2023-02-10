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
#include <render_vulkan/VulkanRenderModule.h>
#include <render_vulkan/VulkanUniformBuffer.h>
#include <render_vulkan/resources/VulkanMaterial.h>
#include <render_vulkan/VulkanCommandBuffer.h>

#include <map>
#include <unordered_map>

struct FrameRenderData
{
    glm::vec3 lightPos;
    glm::vec3 lightColor;
    float lightIntensity;
    float time;
    glm::vec2 screenResolution;

    bool uploaded;
    VkDescriptorBufferInfo fubi;
};

struct CameraRenderData
{
    glm::mat4 matCameraToProjection;
    glm::mat4 matWorldToProjection;
    glm::quat cameraRot;
    glm::vec3 cameraPos;

    int cameraId;
    VkDescriptorBufferInfo cubi;
};

struct EntityRenderData
{
    glm::vec3 pos;
    glm::quat rot;
    blink::IGeometry* geometry;
    uint32_t renderLayer;
    int cameraId;
    VkDescriptorBufferInfo eubi;
};

struct MaterialRenderData
{
    int cameraId{};
    VkDescriptorBufferInfo mubi{};
    std::vector<EntityRenderData> entityRenderData;
};

void uploadFrameUniforms(FrameRenderData& frd, blink::VulkanMaterial* vulkanMaterial, const blink::VulkanRenderData* vulkanRenderData)
{
    if (frd.uploaded)
    {
        vulkanMaterial->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Frame, frd.fubi);
        return;
    }

    auto& pipeline = vulkanMaterial->getPipeline();
    auto uniformBlock = pipeline.getUniformBlock(blink::UniformBinding::Frame);
    if (!uniformBlock) return;

    // upload per frame uniforms
    uniformBlock->prepareBuffer();
    uniformBlock->setUniformMember("lightPos", &frd.lightPos);
    uniformBlock->setUniformMember("lightColor", &frd.lightColor);
    uniformBlock->setUniformMember("screenResolution", &frd.screenResolution);
    uniformBlock->setUniformMember("time", &frd.time);
    uniformBlock->setUniformMember("lightIntensity", &frd.lightIntensity);
    vulkanRenderData->fub->appendData(uniformBlock->getBufferData(), uniformBlock->getBufferSize(), &frd.fubi);
    vulkanMaterial->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Frame, frd.fubi);

    frd.uploaded = true;
}

void uploadCameraUniforms(CameraRenderData& crd,
                          int cameraId,
                          blink::VulkanMaterial* vulkanMaterial,
                          const blink::VulkanRenderData* vulkanRenderData)
{
    if (crd.cameraId == cameraId)
    {
        vulkanMaterial->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Camera, crd.cubi);
        return;
    }

    auto& pipeline = vulkanMaterial->getPipeline();
    auto uniformBlock = pipeline.getUniformBlock(blink::UniformBinding::Camera);
    if (!uniformBlock) return;

    auto cameraDir = glm::rotate(crd.cameraRot, glm::forward());
    auto up = glm::rotate(crd.cameraRot, glm::up());
    auto matWorldToCamera = glm::lookAt(crd.cameraPos, crd.cameraPos + cameraDir, up);
    auto matWorldToCameraInvT = glm::transpose(glm::inverse(glm::mat3(matWorldToCamera)));
    crd.matWorldToProjection = crd.matCameraToProjection * matWorldToCamera;

    // upload per camera uniforms
    uniformBlock->prepareBuffer();
    uniformBlock->setUniformMember("matWorldToCamera", &matWorldToCamera);
    uniformBlock->setUniformMember("matCameraToProjection", &crd.matCameraToProjection);
    uniformBlock->setUniformMember("matWorldToProjection", &crd.matWorldToProjection);
    uniformBlock->setUniformMember("matWorldToCameraInvT", &matWorldToCameraInvT);
    uniformBlock->setUniformMember("cameraPos", &crd.cameraPos);
    uniformBlock->setUniformMember("cameraDir", &cameraDir);
    vulkanRenderData->fub->appendData(uniformBlock->getBufferData(), uniformBlock->getBufferSize(), &crd.cubi);
    vulkanMaterial->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Camera, crd.cubi);

    crd.cameraId = cameraId;
}

void uploadEntityUniforms(EntityRenderData& erd,
                          const CameraRenderData& pcu,
                          int cameraId,
                          blink::VulkanMaterial* vulkanMaterial,
                          const blink::VulkanRenderData* vulkanRenderData)
{
    // bind per instance uniform
    if (erd.cameraId == cameraId)
    {
        vulkanMaterial->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Entity, erd.eubi);
        return;
    }

    auto& pipeline = vulkanMaterial->getPipeline();
    auto uniformBlock = pipeline.getUniformBlock(blink::UniformBinding::Entity);
    if (!uniformBlock) return;

    auto matLocalToWorld = glm::translate(glm::identity<glm::mat4>(), erd.pos) * glm::mat4_cast(erd.rot);
    auto matLocalToWorldInvT = glm::transpose(glm::inverse(glm::mat3(matLocalToWorld)));
    auto matLocalToProjection = pcu.matWorldToProjection * matLocalToWorld;

    uniformBlock->prepareBuffer();
    uniformBlock->setUniformMember("matLocalToWorld", &matLocalToWorld);
    uniformBlock->setUniformMember("matLocalToWorldInvT", &matLocalToWorldInvT);
    uniformBlock->setUniformMember("matLocalToProjection", &matLocalToProjection);
    vulkanRenderData->eub->appendData(uniformBlock->getBufferData(), uniformBlock->getBufferSize(), &erd.eubi);
    vulkanMaterial->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Entity, erd.eubi);

    erd.cameraId = cameraId;
}

void uploadMaterialUniforms(MaterialRenderData& mrd,
                            int cameraId,
                            blink::VulkanMaterial* vulkanMaterial,
                            const blink::VulkanRenderData* vulkanRenderData)
{
    // bind per material uniform
    if (mrd.cameraId == cameraId)
    {
        vulkanMaterial->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Material, mrd.mubi);
        return;
    }

    auto& pipeline = vulkanMaterial->getPipeline();
    auto uniformBlock = pipeline.getUniformBlock(blink::UniformBinding::Material);
    if (!uniformBlock) return;

    vulkanRenderData->mub->appendData(uniformBlock->getBufferData(), uniformBlock->getBufferSize(), &mrd.mubi);
    vulkanMaterial->uploadUniformDescriptorBufferInfo(blink::UniformBinding::Material, mrd.mubi);

    mrd.cameraId = cameraId;
}

SceneRenderSystem::SceneRenderSystem(blink::IApp* app)
    : m_app(app)
{
    //
}

bool SceneRenderSystem::initialize() { return true; }

void SceneRenderSystem::terminate()
{
    //
}

void SceneRenderSystem::render(blink::IRenderData& renderData)
{
    const auto& world = m_app->getEcsWorld().getWorld();
    auto renderModule = blink::getRenderModule();
    const auto& surfaceSize = renderModule->getSurfaceSize();

    // collect light data
    FrameRenderData frd{};
    {
        frd.time = std::fmodf(world.time(), 3600.0f);
        frd.screenResolution = surfaceSize;
        static auto lightDataQuery = world.query_builder<const blink::Position, const blink::LightData>().build();
        lightDataQuery.each(
            [&frd](flecs::entity e, const blink::Position& pos, const blink::LightData& light)
            {
                frd.lightPos = pos.value;
                frd.lightColor = light.color;
                frd.lightIntensity = light.intensity;
            });
    }

    // collect camera data into cameraGroups
    std::vector<CameraRenderData> cameraGroups;
    {
        static auto cameraDataQuery =
            world.query_builder<const blink::Position, const blink::Rotation, const blink::CameraData>().build();
        cameraDataQuery.each(
            [&cameraGroups](flecs::entity e,
                            const blink::Position& pos,
                            const blink::Rotation& rot,
                            const blink::CameraData& camera)
            {
                // setup perFrame uniforms
                CameraRenderData pcu{};

                pcu.cameraPos = pos.value;
                pcu.cameraRot = rot.value;
                pcu.matCameraToProjection = camera.matCameraToProjection;
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
                    findIt->second.entityRenderData.push_back({pos.value, rot.value, geometry, renderable.renderLayer, 0, {}});
                }
                else
                {
                    MaterialRenderData mrd;
                    mrd.cameraId = 0;
                    mrd.entityRenderData.push_back({pos.value, rot.value, geometry, renderable.renderLayer, 0, {}});
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
                if (findIt != featureGroups.end()) { LOGE("duplicate render feature order {0}", renderFeature.order); }
                else { featureGroups.emplace(renderFeature.order, renderFeature); }
            });
    }

    auto vulkanRenderData = static_cast<blink::VulkanRenderData*>(&renderData);

    // start rendering

    // for each camera
    int cameraId = 1;
    for (auto& crd : cameraGroups)
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
                    auto& mrd = kvpMaterial.second;

                    auto vulkanMaterial = dynamic_cast<blink::VulkanMaterial*>(material);

                    vulkanMaterial->bindPipeline(*vulkanRenderData->commandBuffer);

                    uploadFrameUniforms(frd, vulkanMaterial, vulkanRenderData);
                    uploadCameraUniforms(crd, cameraId, vulkanMaterial, vulkanRenderData);
                    uploadMaterialUniforms(mrd, cameraId, vulkanMaterial, vulkanRenderData);

                    for (auto& erd : mrd.entityRenderData)
                    {
                        if ((erd.renderLayer & renderFeatureData.renderLayer) == 0) continue;

                        uploadEntityUniforms(erd, crd, cameraId, vulkanMaterial, vulkanRenderData);

                        // update vertex input and uniform buffers
                        vulkanMaterial->updateBufferInfos(*vulkanRenderData->commandBuffer, static_cast<blink::VulkanGeometry*>(erd.geometry));

                        vkCmdDrawIndexed(*vulkanRenderData->commandBuffer, erd.geometry->getNumIndices(), 1, 0, 0, 0);
                    }
                }
            }
            else
            {
                blink::IMaterial* material = renderFeatureData.overrideMaterial;
                auto vulkanMaterial = dynamic_cast<blink::VulkanMaterial*>(material);

                vulkanMaterial->bindPipeline(*vulkanRenderData->commandBuffer);

                uploadFrameUniforms(frd, vulkanMaterial, vulkanRenderData);
                uploadCameraUniforms(crd, cameraId, vulkanMaterial, vulkanRenderData);
                MaterialRenderData mrd{};
                uploadMaterialUniforms(mrd, cameraId, vulkanMaterial, vulkanRenderData);

                // render mesh group by material
                for (auto& kvpMaterial : materialGroups)
                {
                    for (auto& erd : kvpMaterial.second.entityRenderData)
                    {
                        if ((erd.renderLayer & renderFeatureData.renderLayer) == 0) continue;

                        uploadEntityUniforms(erd, crd, cameraId, vulkanMaterial, vulkanRenderData);

                        // update vertex input and uniform buffers
                        vulkanMaterial->updateBufferInfos(*vulkanRenderData->commandBuffer, static_cast<blink::VulkanGeometry*>(erd.geometry));

                        vkCmdDrawIndexed(*vulkanRenderData->commandBuffer, erd.geometry->getNumIndices(), 1, 0, 0, 0);
                    }
                }
            }
        }

        ++cameraId;
    }
}
