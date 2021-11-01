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

namespace blink
{
    class ISystemBase
    {
    public:
        virtual bool initialize(flecs::world& world) = 0;
        virtual void terminate(flecs::world& world){};
    };

} // namespace blink
