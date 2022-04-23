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
        void render(IRenderData& renderData);

        EcsWorld& getEcsWorld() { return m_ecsWorld; };

    protected:
        bool addRenderSystem(IRenderSystem* sys);
        bool initializeRenderSystems();
        void terminateRenderSystems();
        void destroyRenderSystems();

    protected:
        EcsWorld m_ecsWorld;
        std::chrono::steady_clock::time_point m_lastTime;
        float m_deltaTime{};
        float m_elapseTime{};

        std::vector<IRenderSystem*> m_renderSystems;
    };

} // namespace blink
