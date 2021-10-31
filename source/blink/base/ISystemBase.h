/**

    @file      ISystemBase.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      31.10.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <flecs/flecs.h>
#include <foundation/BaseTypesGlm.h>

NS_BEGIN

class ISystemBase
{
public:
    virtual bool initialize(flecs::world& world) { return true; };
    virtual void terminate(flecs::world& world){};
};

NS_END
