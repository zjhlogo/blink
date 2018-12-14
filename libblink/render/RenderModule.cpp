#include "RenderModule.h"
#include "GlConfig.h"
#include "glad/glad.h"

namespace blink
{
    RenderModule::RenderModule()
    {
        m_currentRenderState.m_state = RenderState::RS_DEPTH_TEST | RenderState::RS_DEPTH_WRITE;
        m_currentRenderState.m_blendMode = RenderState::BlendMode::Disable;
        m_currentRenderState.m_cullMode = RenderState::CullMode::Back;
    }

    RenderModule::~RenderModule()
    {

    }

    bool RenderModule::initialize(Entity* pEntity)
    {
        LOGI("Version: {0}", glGetString(GL_VERSION));
        LOGI("Vendor: {0}", glGetString(GL_VENDOR));
        LOGI("Renderer: {0}", glGetString(GL_RENDERER));

        //const GLubyte* pszExtensions = glGetString(GL_EXTENSIONS);
        //StringList extensionsArray;
        //StringUtil::splitString(extensionsArray, reinterpret_cast<const char*>(pszExtensions), " ");
        //for (const auto& ext : extensionsArray)
        //{
        //    LOGI("Extensions: {0}", ext);
        //}

        LOG_GL_INT_CAPACITY(GL_DEPTH_BITS);
        LOG_GL_INT_CAPACITY(GL_STENCIL_BITS);

        LOG_GL_INT_CAPACITY(GL_MAX_VERTEX_ATTRIBS);					// The value must be at least 8
        LOG_GL_INT_CAPACITY(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);		// The value may be 0
        LOG_GL_INT_CAPACITY(GL_MAX_TEXTURE_IMAGE_UNITS);			// The value must be at least 8

        LOG_GL_INT_CAPACITY(GL_MAX_TEXTURE_SIZE);					// The value must be at least 64
        LOG_GL_INT_CAPACITY(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);	// The value must be at least 8
        LOG_GL_INT_CAPACITY(GL_MAX_CUBE_MAP_TEXTURE_SIZE);			// The value must be at least 16
        LOG_GL_INT_CAPACITY(GL_MAX_RENDERBUFFER_SIZE);				// The value must be at least 1

        return true;
    }

    void RenderModule::terminate()
    {

    }

    void RenderModule::reset()
    {
        applyCurrentRenderState(true);
        m_currentRenderState.m_viewport = glm::ivec4(0, 0, m_surfaceSize.x, m_surfaceSize.y);

        glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void RenderModule::applyCurrentRenderState(bool force /* = true */)
    {
        if (force || m_deviceRenderState.m_state != m_currentRenderState.m_state)
        {
            if (force || (m_deviceRenderState.m_state & RenderState::RS_SCISSOR_TEST) != (m_currentRenderState.m_state & RenderState::RS_SCISSOR_TEST))
            {
                if (m_currentRenderState.m_state & RenderState::RS_SCISSOR_TEST)
                {
                    glEnable(GL_SCISSOR_TEST);
                    glScissor(m_currentRenderState.m_scissor.x, m_currentRenderState.m_scissor.y, m_currentRenderState.m_scissor.z, m_currentRenderState.m_scissor.w);
                }
                else
                {
                    glDisable(GL_SCISSOR_TEST);
                }
            }

            if (force || (m_deviceRenderState.m_state & RenderState::RS_DEPTH_TEST) != (m_currentRenderState.m_state & RenderState::RS_DEPTH_TEST))
            {
                if (m_currentRenderState.m_state & RenderState::RS_DEPTH_TEST)
                {
                    glEnable(GL_DEPTH_TEST);
                    glDepthFunc(GL_LEQUAL);
                }
                else
                {
                    glEnable(GL_DEPTH_TEST);
                    glDepthFunc(GL_ALWAYS);
                }
            }

            if (force || (m_deviceRenderState.m_state & RenderState::RS_DEPTH_WRITE) != (m_currentRenderState.m_state & RenderState::RS_DEPTH_WRITE))
            {
                if (m_currentRenderState.m_state & RenderState::RS_DEPTH_WRITE)
                {
                    glDepthMask(GL_TRUE);
                }
                else
                {
                    glDepthMask(GL_FALSE);
                }
            }

            m_deviceRenderState.m_state = m_currentRenderState.m_state;
        }

        if (force || m_deviceRenderState.m_viewport != m_currentRenderState.m_viewport)
        {
            glViewport(m_currentRenderState.m_viewport.x, m_currentRenderState.m_viewport.y, m_currentRenderState.m_viewport.z, m_currentRenderState.m_viewport.w);
            m_deviceRenderState.m_viewport = m_currentRenderState.m_viewport;
        }

        if (force || m_deviceRenderState.m_blendMode != m_currentRenderState.m_blendMode)
        {
            switch (m_currentRenderState.m_blendMode)
            {
            case RenderState::BlendMode::Disable:
                glDisable(GL_BLEND);
                break;
            case RenderState::BlendMode::Normal:
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                break;
            case RenderState::BlendMode::Additive:
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE);
                break;
            default:
                break;
            }

            m_deviceRenderState.m_blendMode = m_currentRenderState.m_blendMode;
        }

        if (force || m_deviceRenderState.m_cullMode != m_currentRenderState.m_cullMode)
        {
            switch (m_currentRenderState.m_cullMode)
            {
            case RenderState::CullMode::Disable:
                glDisable(GL_CULL_FACE);
                break;
            case RenderState::CullMode::Front:
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);
                break;
            case RenderState::CullMode::Back:
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
                break;
            default:
                break;
            }

            m_deviceRenderState.m_cullMode = m_currentRenderState.m_cullMode;
        }
    }
}
