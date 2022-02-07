/**

    @file      AngularVelocitySystem.h
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
    class AngularVelocitySystem : public ILogicalSystem
    {
    public:
        virtual bool initialize(flecs::world& world) override;
        virtual void terminate(flecs::world& world) override;
    };
} // namespace blink
