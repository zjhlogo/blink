#pragma once
#include <glm/glm.hpp>

namespace blink
{
    class RenderState
    {
    public:
        static const int RS_DEPTH_TEST = 1 << 0;
        static const int RS_DEPTH_WRITE = 1 << 1;
        static const int RS_SCISSOR_TEST = 1 << 2;

        enum class BlendMode
        {
            Disable = 0,
            Normal,
            Additive
        };

        enum class CullMode
        {
            Disable = 0,
            Front,
            Back
        };

        class StateInfo
        {
        public:
            int m_state{};
            glm::ivec4 m_viewport;
            glm::ivec4 m_scissor;
            BlendMode m_blendMode{ BlendMode::Disable };
            CullMode m_cullMode{ CullMode::Disable };
        };

    public:
        RenderState();
        ~RenderState();

        void setViewport(int x, int y, int w, int h);
        void setScissorSize(int x, int y, int w, int h);
        void setBlendMode(BlendMode blendMode);
        void setCullMode(CullMode cullMode);

        bool setDepthTest(bool enable);
        bool setDepthWritable(bool enable);
        bool setScissorTest(bool enable);

    private:
        bool enableState(int state) { int oldState = m_pCurStateInfo->m_state; m_pCurStateInfo->m_state |= state; return (oldState != m_pCurStateInfo->m_state); };
        bool disableState(int state) { int oldState = m_pCurStateInfo->m_state; m_pCurStateInfo->m_state &= (~state); return (oldState != m_pCurStateInfo->m_state); };
        bool checkState(int state) const { return ((m_pCurStateInfo->m_state & state) == state); };

    private:
        StateInfo* m_pCurStateInfo{};
        StateInfo m_oldStateInfo;

    };
}
