/**

    @file      app.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "app.h"

namespace blink
{
    IApp::IApp()
    {
        //
        m_lastTime = std::chrono::steady_clock::now();
    }

    IApp::~IApp()
    {
        m_ecsWorld.destroyAllSystems();
    }

    void IApp::stepEcsWorld()
    {
        auto currTime = std::chrono::steady_clock::now();
        m_deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - m_lastTime).count() / 1000.0f;
        m_lastTime = currTime;

        m_elapseTime += m_deltaTime;
        while (m_elapseTime >= EcsWorld::FIXED_DT)
        {
            m_elapseTime = std::fmodf(m_elapseTime, EcsWorld::FIXED_DT);
            m_ecsWorld.step();
        }
    }
} // namespace blink
