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

#include <blink/base/ISystemBase.h>
#include <blink/component/Components.h>
#include <render_vulkan/VulkanCommandBuffer.h>
#include <render_vulkan/VulkanCommandPool.h>
#include <render_vulkan/VulkanRenderModule.h>
#include <render_vulkan/VulkanUniformBuffer.h>

#include <unordered_map>

namespace blink
{
    IApp::~IApp() { destroyAllSystems(); }

    void IApp::update(float dt) { m_world.progress(dt); }

    struct RenderData
    {
        glm::vec3 pos;
        glm::quat rot;
        Geometry* geometry;
    };

    void IApp::render(VulkanCommandBuffer& commandBuffer, VulkanUniformBuffer& uniformBuffer)
    {
        // group render object by material
        std::unordered_map<Material*, std::vector<RenderData>> renderDatas;
        m_world.each(
            [&](flecs::entity e, const Position& pos, const Rotation& rot, const StaticModel& model)
            {
                auto findIt = renderDatas.find(model.material);
                if (findIt != renderDatas.end())
                {
                    findIt->second.push_back({pos.value, rot.value, model.geometry});
                }
                else
                {
                    std::vector<RenderData> dataLists;
                    dataLists.push_back({pos.value, rot.value, model.geometry});
                    renderDatas.emplace(model.material, dataLists);
                }
            });

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
