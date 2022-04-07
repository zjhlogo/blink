/**

    @file      LinearVelocitySystem.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      8.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once
#include "../base/ILogicalSystem.h"

namespace blink
{
    class LinearVelocitySystem : public ILogicalSystem
    {
    public:
        virtual bool initialize() override;
        virtual void terminate() override;
    };
} // namespace blink
