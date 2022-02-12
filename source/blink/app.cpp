/**

    @file      app.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "app.h"

#include <blink/base/ILogicalSystem.h>
#include <blink/base/IRenderSystem.h>

#include <map>

namespace blink
{
    IApp::~IApp()
    {
        //
        destroyRenderSystems();
        destroyLogicalSystems();
    }

    void IApp::executeLogicalSystems()
    {
        //
        m_world.progress();
    }

    void IApp::executeRenderSystems(VulkanCommandBuffer& commandBuffer, VulkanUniformBuffer& pfub, VulkanUniformBuffer& pmub, VulkanUniformBuffer& piub)
    {
        for (auto sys : m_renderSystems)
        {
            sys->render(commandBuffer, pfub, pmub, piub);
        }
    }

    bool IApp::addLogicalSystem(ILogicalSystem* sys)
    {
        m_logicalSystems.push_back(sys);

        return true;
    }

    bool IApp::initializeLogicalSystems()
    {
        for (auto sys : m_logicalSystems)
        {
            if (!sys->initialize(m_world)) return false;
        }

        return true;
    }

    void IApp::terminateLogicalSystems()
    {
        for (int i = (int)m_logicalSystems.size() - 1; i >= 0; --i)
        {
            m_logicalSystems[i]->terminate(m_world);
        }
    }

    void IApp::destroyLogicalSystems()
    {
        for (int i = (int)m_logicalSystems.size() - 1; i >= 0; --i)
        {
            SAFE_DELETE(m_logicalSystems[i]);
        }

        m_logicalSystems.clear();
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
