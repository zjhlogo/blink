/*********************************************************************
 * \file   IRenderModule.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#pragma once
#include "../IRenderSystem.h"

#include <glm/glm.hpp>

#include <functional>
#include <vector>

namespace blink
{
    class IRenderModule
    {
    public:
        IRenderModule() = default;
        virtual ~IRenderModule() = default;

        IRenderModule(const IRenderModule& renderModule) = delete;
        IRenderModule(IRenderModule&& renderModule) = delete;
        IRenderModule& operator=(const IRenderModule& renderModule) = delete;
        IRenderModule& operator=(IRenderModule&& renderModule) = delete;

        virtual bool createDevice(const glm::ivec2& deviceSize) = 0;
        virtual void destroyDevice() = 0;

        virtual bool processEvent() = 0;
        virtual void render() = 0;

        virtual glm::vec2 getSurfaceSize() const = 0;

        virtual void waitIdle() = 0;

        IRenderSystem* addRenderSystem(IRenderSystem* sys);
        bool initializeRenderSystems();
        void terminateRenderSystems();

    protected:
        void destroyRenderSystems();

    protected:
        std::vector<IRenderSystem*> m_renderSystems;
    };

    IRenderModule* getRenderModule();
} // namespace blink
