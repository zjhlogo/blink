/**

    @file      IRenderSystem.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

namespace blink
{
    class IRenderData { };

    class IRenderSystem
    {
    public:
        IRenderSystem() = default;
        virtual ~IRenderSystem() = default;

        IRenderSystem(const IRenderSystem& system) = delete;
        IRenderSystem(IRenderSystem&& system) = delete;
        IRenderSystem& operator=(const IRenderSystem& system) = delete;
        IRenderSystem& operator=(IRenderSystem&& system) = delete;

        virtual bool initialize() = 0;
        virtual void terminate() = 0;
        virtual void render(IRenderData& renderData) = 0;
    };
} // namespace blink
