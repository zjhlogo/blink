#include "RenderState.h"
#include "RenderModule.h"
#include "../Framework.h"

namespace blink
{
    RenderState::RenderState()
    {
        RenderModule* pRenderModule = Framework::getInstance().findComponent<RenderModule>();
        m_oldStateInfo = pRenderModule->m_currentRenderState;
        m_pCurStateInfo = &pRenderModule->m_currentRenderState;
    }

    RenderState::~RenderState()
    {
        RenderModule* pRenderModule = Framework::getInstance().findComponent<RenderModule>();
        pRenderModule->m_currentRenderState = m_oldStateInfo;
    }

    void RenderState::setViewport(int x, int y, int w, int h)
    {
        m_pCurStateInfo->m_viewport = glm::ivec4(x, y, w, h);
    }

    void RenderState::setScissorSize(int x, int y, int w, int h)
    {
        m_pCurStateInfo->m_scissor = glm::ivec4(x, y, w, h);
    }

    void RenderState::setBlendMode(BlendMode blendMode)
    {
        m_pCurStateInfo->m_blendMode = blendMode;
    }

    void RenderState::setCullMode(CullMode cullMode)
    {
        m_pCurStateInfo->m_cullMode = cullMode;
    }

    bool RenderState::setDepthTest(bool enable)
    {
        if (enable)
        {
            return enableState(RS_DEPTH_TEST);
        }
        else
        {
            return disableState(RS_DEPTH_TEST);
        }

        return false;
    }

    bool RenderState::setDepthWritable(bool enable)
    {
        if (enable)
        {
            return enableState(RS_DEPTH_WRITE);
        }
        else
        {
            return disableState(RS_DEPTH_WRITE);
        }

        return false;
    }

    bool RenderState::setScissorTest(bool enable)
    {
        if (enable)
        {
            return enableState(RS_SCISSOR_TEST);
        }
        else
        {
            return disableState(RS_SCISSOR_TEST);
        }

        return false;
    }
}
