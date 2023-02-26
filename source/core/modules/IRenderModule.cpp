/*********************************************************************
 * \file   IRenderModule.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#include "IRenderModule.h"

#include <foundation/PreDefineMacro.h>

namespace blink
{
    IRenderSystem* IRenderModule::addRenderSystem(IRenderSystem* sys)
    {
        m_renderSystems.push_back(sys);
        return sys;
    }

    bool IRenderModule::initializeRenderSystems()
    {
        for (auto sys : m_renderSystems)
        {
            if (!sys->initialize()) return false;
        }

        return true;
    }

    void IRenderModule::terminateRenderSystems()
    {
        for (int i = (int)m_renderSystems.size() - 1; i >= 0; --i)
        {
            m_renderSystems[i]->terminate();
        }
    }

    void IRenderModule::destroyRenderSystems()
    {
        for (int i = (int)m_renderSystems.size() - 1; i >= 0; --i)
        {
            SAFE_DELETE(m_renderSystems[i]);
        }

        m_renderSystems.clear();
    }
} // namespace blink
