/**

    @file      app.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "app.h"
#include "geometry/Geometry.h"
#include "material/Material.h"
#include "resource/ResourceMgr.h"
#include "type/RenderTypes.h"

#include <blink/component/Components.h>
#include <blink/system/ISystemBase.h>
#include <foundation/Log.h>
#include <glm/gtx/quaternion.hpp>
#include <render_vulkan/VulkanCommandBuffer.h>
#include <render_vulkan/VulkanUniformBuffer.h>

#include <map>

namespace blink
{
    IApp::~IApp() { destroyAllSystems(); }

    void IApp::update(float dt) { m_world.progress(dt); }

    void IApp::render(VulkanCommandBuffer& commandBuffer, VulkanUniformBuffer& uniformBuffer)
    {
        // collect camera data into pfus
        std::vector<PerFrameUniforms> pfus;
        m_world.each(
            [&pfus](flecs::entity e, const Position& pos, const Rotation& rot, const CameraData& camera)
            {
                // setup perframe uniforms
                PerFrameUniforms pfu;
                pfu.cameraPos = pos.value;

                pfu.cameraDir = glm::rotate(rot.value, glm::vec3(0.0f, 0.0f, 1.0f));
                auto up = glm::rotate(rot.value, glm::vec3(0.0f, 1.0f, 0.0f));
                pfu.matWorldToCamera = glm::lookAt(pfu.cameraPos, pfu.cameraPos + pfu.cameraDir, up);

                pfu.matWorldToCameraInvT = glm::transpose(glm::inverse(glm::mat3(pfu.matWorldToCamera)));
                pfu.matCameraToProjection = glm::perspective(camera.fov, camera.aspect, camera.near, camera.far);
                pfu.matWorldToProjection = pfu.matCameraToProjection * pfu.matWorldToCamera;
                pfus.push_back(pfu);
            });

        // group render object by materials
        std::unordered_map<Material*, std::vector<RenderData>> renderDatas;
        m_world.each(
            [&](flecs::entity e, const Position& pos, const Rotation& rot, const StaticModel& model)
            {
                auto material = model.material;
                if (!material) return;

                auto geometry = model.geometry;
                if (!geometry) return;

                auto findIt = renderDatas.find(material);
                if (findIt != renderDatas.end())
                {
                    findIt->second.push_back({pos.value, rot.value, geometry});
                }
                else
                {
                    std::vector<RenderData> dataLists;
                    dataLists.push_back({pos.value, rot.value, geometry});
                    renderDatas.emplace(material, dataLists);
                }
            });

        // group render features
        std::map<int, RenderFeatureData> renderFeatureDatas;
        m_world.each(
            [&](flecs::entity e, const RenderFeature& feature)
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

                    RenderFeatureData renderFeatureData{feature.order, material};
                    renderFeatureDatas.emplace(feature.order, renderFeatureData);
                }
            });

        // start rendering
        for (const auto& pfu : pfus)
        {
            if (!uniformBuffer.alignBufferOffset()) return;
            uniformBuffer.appendPerFrameBufferData(&pfu, sizeof(pfu));

            // render mesh group by material
            for (const auto& kvp : renderDatas)
            {
                Material* material = kvp.first;

                material->bindPipeline(commandBuffer);

                for (const auto& renderData : kvp.second)
                {
                    renderData.geometry->bindBuffer(commandBuffer);
                    material->bindUniformBuffer(commandBuffer, uniformBuffer, renderData.pos, renderData.rot);
                    vkCmdDrawIndexed(commandBuffer, renderData.geometry->getNumIndices(), 1, 0, 0, 0);
                }
            }

            // render features
            for (const auto& kvpFeature : renderFeatureDatas)
            {
                Material* material = kvpFeature.second.material;
                material->bindPipeline(commandBuffer);

                for (const auto& kvp : renderDatas)
                {
                    for (const auto& renderData : kvp.second)
                    {
                        renderData.geometry->bindBuffer(commandBuffer);
                        material->bindUniformBuffer(commandBuffer, uniformBuffer, renderData.pos, renderData.rot);
                        vkCmdDrawIndexed(commandBuffer, renderData.geometry->getNumIndices(), 1, 0, 0, 0);
                    }
                }
            }
        }
    }

    bool IApp::addSystem(ISystemBase* sys)
    {
        m_systems.push_back(sys);

        return false;
    }

    bool IApp::initializeSystems()
    {
        for (auto sys : m_systems)
        {
            if (!sys->initialize(m_world)) return false;
        }

        return true;
    }

    void IApp::terminateSystems()
    {
        for (int i = (int)m_systems.size() - 1; i >= 0; --i)
        {
            m_systems[i]->terminate(m_world);
        }
    }

    void IApp::destroyAllSystems()
    {
        for (int i = (int)m_systems.size() - 1; i >= 0; --i)
        {
            SAFE_DELETE(m_systems[i]);
        }

        m_systems.clear();
    }

} // namespace blink
