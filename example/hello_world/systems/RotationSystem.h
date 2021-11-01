/**

    @file      RotationSystem.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <blink/base/ISystemBase.h>

class RotationSystem : public blink::ISystemBase
{
public:
    virtual bool initialize(flecs::world& world) override;
};
