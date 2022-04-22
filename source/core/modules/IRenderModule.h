
/*********************************************************************
 * \file   IRenderModule.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#pragma once
#include "../IRenderSystem.h"

#include <functional>

namespace blink
{
    class IRenderModule
    {
    public:
        enum RenderResult
        {
            Success,
            Failed,
            Recreate
        };

        typedef std::function<void(IRenderData& renderData)> RenderCb;

    public:
        virtual bool createDevice(const glm::ivec2& deviceSize) = 0;
        virtual void destroyDevice() = 0;

        virtual bool processEvent() = 0;
        virtual RenderResult render(const RenderCb& cb) = 0;

        virtual glm::vec2 getSurfaceSize() const = 0;

        virtual void waitIdle() = 0;
    };

    IRenderModule* getRenderModule();
} // namespace blink
