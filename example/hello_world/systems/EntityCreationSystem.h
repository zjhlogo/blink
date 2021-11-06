/**

    @file      EntityCreationSystem.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <blink/base/ISystemBase.h>

class EntityCreationSystem : public blink::ISystemBase
{
public:
    EntityCreationSystem();

    virtual bool initialize(flecs::world& world) override;

};
