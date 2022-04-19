/**

    @file      app.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "app.h"

#include <render_vulkan/base/IRenderSystem.h>

namespace blink
{
    IApp::IApp()
    {
        //
        m_lastTime = std::chrono::steady_clock::now();
    }

    IApp::~IApp()
    {
        destroyRenderSystems();
        m_ecsWorld.destroyAllSystems();
    }

    void IApp::stepEcsWorld()
    {
        auto currTime = std::chrono::steady_clock::now();
        m_deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - m_lastTime).count() / 1000.0f;
        m_lastTime = currTime;

        m_elapseTime += m_deltaTime;
        while (m_elapseTime > EcsWorld::FIXED_DT)
        {
            m_elapseTime -= ((int)(m_elapseTime / EcsWorld::FIXED_DT) * EcsWorld::FIXED_DT);
            m_ecsWorld.step();
        }
    }

    void IApp::render(VulkanCommandBuffer& commandBuffer,
                      VulkanUniformBuffer& pfub,
                      VulkanUniformBuffer& pmub,
                      VulkanUniformBuffer& piub)
    {
        for (auto sys : m_renderSystems)
        {
            sys->render(commandBuffer, pfub, pmub, piub);
        }
    }

    bool IApp::addRenderSystem(IRenderSystem* sys)
    {
        m_renderSystems.push_back(sys);

        return true;
    }

    bool IApp::initializeRenderSystems()
    {
        for (auto sys : m_renderSystems)
        {
            if (!sys->initialize()) return false;
        }

        return true;
    }

    void IApp::terminateRenderSystems()
    {
        for (int i = (int)m_renderSystems.size() - 1; i >= 0; --i)
        {
            m_renderSystems[i]->terminate();
        }
    }

    void IApp::destroyRenderSystems()
    {
        for (int i = (int)m_renderSystems.size() - 1; i >= 0; --i)
        {
            SAFE_DELETE(m_renderSystems[i]);
        }

        m_renderSystems.clear();
    }

} // namespace blink
