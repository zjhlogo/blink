/**

    @file      AngularVelocitySystem.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      8.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once
#include "ISystemBase.h"

namespace blink
{
    class AngularVelocitySystem : public ISystemBase
    {
    public:
        virtual bool initialize(flecs::world& world) override;
    };
} // namespace blink
