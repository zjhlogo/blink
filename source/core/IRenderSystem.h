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
    class IRenderSystem
    {
    public:
        IRenderSystem() = default;
        virtual ~IRenderSystem() = default;

        IRenderSystem(const IRenderSystem&) = delete;
        IRenderSystem(IRenderSystem&&) = delete;
        IRenderSystem& operator=(const IRenderSystem&) = delete;
        IRenderSystem& operator=(IRenderSystem&&) = delete;

        virtual bool initialize() = 0;
        virtual void terminate() = 0;
        virtual void render() = 0;
    };
} // namespace blink
