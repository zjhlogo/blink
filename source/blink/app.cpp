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
    IApp::~IApp()
    {
        //
        destroyRenderSystems();
        m_ecsWorld.destroyAllSystems();
    }

    void IApp::stepEcsWorld()
    {
        //
        m_ecsWorld.step();
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
