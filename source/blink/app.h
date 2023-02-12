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
#include <core/modules/IRenderModule.h>

#include <chrono>
#include <vector>

namespace blink
{
    class IApp
    {
    public:
        IApp();
        virtual ~IApp();

        virtual bool initialize();
        virtual void terminate();

        void stepEcsWorld();
        EcsWorld& getEcsWorld() { return m_ecsWorld; }

        template <typename T> T* addLogicSystem(T* system)
        {
            m_ecsWorld.addSystem(system);
            return system;
        }

        template <typename T> T* addRenderSystem(T* system)
        {
            m_renderModule->addRenderSystem(system);
            return system;
        }

    protected:
        virtual bool initializeLogicalSystems() = 0;
        virtual bool initializeRenderSystems() = 0;

    private:
        EcsWorld m_ecsWorld;
        IRenderModule* m_renderModule{};

        std::chrono::steady_clock::time_point m_lastTime;
        float m_deltaTime{};
        float m_elapseTime{};
    };

} // namespace blink
