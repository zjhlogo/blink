/**

    @file      ISystemBase.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

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
