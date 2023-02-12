/**

    @file      app.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <core/EcsWorld.h>
#include <core/IRenderSystem.h>

#include <chrono>
#include <vector>

namespace blink
{
    class IApp
    {
    public:
        IApp();
        virtual ~IApp();

        virtual bool initialize() = 0;
        virtual void terminate() = 0;

        void stepEcsWorld();
        EcsWorld& getEcsWorld() { return m_ecsWorld; };

    protected:
        EcsWorld m_ecsWorld;
        std::chrono::steady_clock::time_point m_lastTime;
        float m_deltaTime{};
        float m_elapseTime{};
    };

} // namespace blink
